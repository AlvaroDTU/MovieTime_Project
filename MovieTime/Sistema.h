#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include "ListaDoble.h"
#include "ListaSimple.h"
#include "Cola.h"
#include "Audiovisual.h"
#include "Pelicula.h"
#include "Serie.h"
#include "Categoria.h"
#include "Actor.h"
#include "Director.h"
#include "UsuarioBase.h"
#include "UsuarioCliente.h"
#include "UsuarioAdministrador.h"
#include "GestorArchivos.h"
#include "Ordenamientos.h"
#include "Fondo.h"

class Sistema {
private:
    ListaDoble<Audiovisual*> catalogo;
    ListaDoble<Categoria*> categorias;
    ListaDoble<Actor*> actoresGlobal;
    ListaDoble<Director*> directoresGlobal;
    Cola<Audiovisual*> colaTendencias;
    ListaSimple<UsuarioBase*> usuarios;
    UsuarioBase* usuarioLogueado;

    Fondo objFondo;

    static const int MIN_RATING_TENDENCIA = 8;   // rating >= 8/10
    static const int MIN_POPULARIDAD_TENDENCIA = 5;

  
    void refrescarPantallaConFondo() {
        system("cls");
        objFondo.imprime_movietime_cartelera(0, 0);
        objFondo.gotoxy(1, 12); 
    }

    static void pausar() {
        std::cout << "\nPresione ENTER para continuar...";
        std::cin.get();
    }

    static int leerEntero(const std::string& mensaje) {
        int valor;
        std::cout << mensaje;
        while (!(std::cin >> valor)) {
            if (std::cin.eof()) { std::exit(0); }
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrada invalida. " << mensaje;
        }
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        return valor;
    }

    static double leerDouble(const std::string& mensaje) {
        double valor;
        std::cout << mensaje;
        while (!(std::cin >> valor)) {
            if (std::cin.eof()) { std::exit(0); }
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Entrada invalida. " << mensaje;
        }
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        return valor;
    }

    static std::string leerLinea(const std::string& mensaje) {
        std::string valor;
        std::cout << mensaje;
        std::getline(std::cin, valor);
        return valor;
    }

    // ---------------- Archivos ----------------
    void inicializarDatos() {
        GestorArchivos::cargarCatalogo("catalogo.txt", catalogo, categorias, actoresGlobal, directoresGlobal);
        GestorArchivos::cargarUsuarios("usuarios.txt", usuarios);
        GestorArchivos::cargarResenias("resenias.txt", catalogo, usuarios);
        actualizarTendencias();
    }

    void guardarTodo() {
        GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);
        GestorArchivos::guardarPersonas("personas.txt", actoresGlobal, directoresGlobal);
        GestorArchivos::guardarResenias("resenias.txt", catalogo);
        GestorArchivos::guardarUsuarios("usuarios.txt", usuarios);
    }

    void actualizarTendencias() {
        catalogo.recorrer([this](Audiovisual* item) {
            bool califica = (item->getRating() >= MIN_RATING_TENDENCIA) ||
                (item->getPopularidad() > MIN_POPULARIDAD_TENDENCIA);
            bool yaEsta = colaTendencias.contiene([item](Audiovisual* a) { return a->getId() == item->getId(); });
            if (califica && !yaEsta) colaTendencias.insertar(item);
            });
    }

    // ---------------- Registro de Usuarios ----------------
    void registrarUsuario() {
        refrescarPantallaConFondo();
        std::cout << "========================================\n";
        std::cout << "        REGISTRO DE NUEVO USUARIO        \n";
        std::cout << "========================================\n";

        std::string correo = leerLinea("Ingrese correo electronico: ");

        UsuarioBase** existe = usuarios.buscarSi([&correo](UsuarioBase* u) {
            return u->getCorreo() == correo;
            });

        if (existe != nullptr) {
            std::cout << "\n[!] El correo ya se encuentra registrado \nen el sistema.\n";
            pausar();
            return;
        }

        std::string contra = leerLinea("Ingrese contrasena: ");
        std::string nombres = leerLinea("Ingrese nombres: ");
        std::string apellidos = leerLinea("Ingrese apellidos: ");
        std::string nacionalidad = leerLinea("Ingrese nacionalidad: ");

        std::string nuevoId = std::to_string(usuarios.getLongitud() + 1);
        std::string fechaRegistro = "2026-10-02";

        UsuarioCliente* nuevoCliente = new UsuarioCliente(
            nuevoId,
            nombres,
            apellidos,
            nacionalidad,
            contra,
            correo,
            fechaRegistro
        );

        usuarios.agregarFinal(nuevoCliente);
        GestorArchivos::guardarUsuarios("usuarios.txt", usuarios);

        std::cout << "\n[OK] Registro exitoso! Ya puede iniciar sesion \ncon sus credenciales.\n";
        pausar();
    }

    // ---------------- Sesion ----------------
    bool iniciarSesion() {
        refrescarPantallaConFondo();

        std::cout << "========================================\n";
        std::cout << "      SISTEMA DE STREAMING - MOVIE TIME    \n";
        std::cout << "========================================\n";
        std::cout << "1. Iniciar Sesion\n";
        std::cout << "2. Registrarse\n";
        std::cout << "3. Salir\n";
        std::cout << "========================================\n";

        int opcion = leerEntero("Seleccione una opcion: ");

        if (opcion == 2) {
            registrarUsuario();
            return false;
        }
        else if (opcion == 3) {
            std::exit(0);
        }
        else if (opcion != 1) {
            std::cout << "\n[!] Opcion no valida.\n";
            pausar();
            return false;
        }

        refrescarPantallaConFondo();
        std::cout << "========================================\n";
        std::cout << "             INICIO DE SESION            \n";
        std::cout << "========================================\n";
        std::string correo = leerLinea("Correo: ");
        std::string contra = leerLinea("Contrasena: ");

        UsuarioBase** encontrado = usuarios.buscarSi([&correo, &contra](UsuarioBase* u) {
            return u->autenticar(correo, contra);
            });

        if (encontrado != nullptr) {
            usuarioLogueado = *encontrado;
            std::cout << "\n[OK] Bienvenido(a), " << usuarioLogueado->getNombres() << "!\n";
            pausar();
            return true;
        }
        std::cout << "\n[!] Credenciales incorrectas.\n";
        pausar();
        return false;
    }

    void mostrarCatalogoFicha() {
        if (catalogo.esVacia()) {
            refrescarPantallaConFondo();
            std::cout << "El catalogo esta vacio.\n";
            pausar();
            return;
        }

        std::vector<Audiovisual*> vista;
        catalogo.recorrer([&vista](Audiovisual* a) { vista.push_back(a); });

        size_t indice = 0;
        char opcion = ' ';

        do {
            refrescarPantallaConFondo();
            Audiovisual* actual = vista[indice];

            std::cout << "==================== FICHA " << (indice + 1) << "/" << vista.size() << " ====================\n\n";
            actual->mostrar();
            std::cout << "\nDirector(es): ";
            actual->getDirectores().recorrer([](Director* d) { std::cout << d->getNombreCompleto() << "  "; });
            std::cout << "\nActor(es): ";
            actual->getActores().recorrer([](Actor* a) { std::cout << a->getNombreCompleto() << "  "; });
            std::cout << "\nResenas registradas: " << actual->getResenias().getLongitud() << "\n";

            std::cout << "\n----------------------------------------\n";
            std::cout << "[D] Siguiente   [A] Anterior   [R] Dejar Resena  \n [V] Ver Resenas   [Q] Volver\n";
            std::cout << "Opcion: ";
            std::cin >> opcion;
            opcion = toupper(opcion);

            switch (opcion) {
            case 'D':
                indice = (indice + 1) % vista.size();
                break;
            case 'A':
                indice = (indice == 0) ? vista.size() - 1 : indice - 1;
                break;
            case 'R':
                dejarResenia(actual);
                break;
            case 'V':
                verResenias(actual);
                break;
            case 'Q':
                break;
            default:
                std::cout << "Opcion no valida.\n";
                pausar();
            }
        } while (opcion != 'Q');
    }

    void verResenias(Audiovisual* item) {
        refrescarPantallaConFondo();
        std::cout << "=== RESEnAS DE \"" << item->getTitulo() << "\" ===\n\n";
        if (item->getResenias().esVacia()) {
            std::cout << "Aun no hay resenas para este contenido.\n";
        }
        else {
            item->getResenias().recorrer([](Resenia* r) { r->mostrar(); });
        }
        pausar();
        pausar();
    }

    void dejarResenia(Audiovisual* item) {
        UsuarioCliente* cliente = static_cast<UsuarioCliente*>(usuarioLogueado);
        if (!cliente) {
            std::cout << "\nSolo los usuarios clientes pueden dejar resenas.\n";
            pausar();
            return;
        }

        refrescarPantallaConFondo();
        std::cout << "=== DEJAR RESEnA: \"" << item->getTitulo() << "\" ===\n\n";
        double calif = leerDouble("Calificacion (0 al 10): ");
        std::string comentario = leerLinea("Comentario: ");

        Resenia* nueva = new Resenia(item->getId(), cliente->getId(), calif, comentario);
        item->agregarResenia(nueva);
        cliente->agregarResenia(nueva);

        actualizarTendencias();
        GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);
        GestorArchivos::guardarResenias("resenias.txt", catalogo);

        std::cout << "\n[OK] Resena agregada. Nuevo rating: " << item->getRating() << "/10\n";
        pausar();
    }

    // ---------------- Tendencias ----------------
    void mostrarTendencias() {
        refrescarPantallaConFondo();
        std::cout << "=== TENDENCIAS DEL DIA ===\n\n";
        if (colaTendencias.empty())
            std::cout << "No hay tendencias registradas todavia.\n";
        else
        {
            int puesto = 1;
            colaTendencias.recorrer([&puesto](Audiovisual* a) {
                std::cout << puesto++ << ". " << a->getTitulo()
                    << " | Rating: " << a->getRating() << "/10"
                    << " | Popularidad: " << a->getPopularidad() << "\n";
                });
        }
        pausar();
    }


    void buscarPorActor() {
        refrescarPantallaConFondo();
        std::cout << "=== BUSCAR CONTENIDO POR ACTOR ===\n";
        std::string nombreActor = leerLinea("Nombre del actor a buscar: ");

        // Búsqueda recursiva en la ListaDoble evaluando el predicado lambda
        Audiovisual** res = catalogo.buscarRecursivo([&nombreActor](Audiovisual* a) {
            return a != nullptr && a->tieneActor(nombreActor);
            });

        refrescarPantallaConFondo();
        if (res && *res) {
            std::cout << "=== RESULTADO ENCONTRADO ===\n\n";
            (*res)->mostrar();
        }
        else {
            std::cout << "No se encontro ningun contenido con el actor: " << nombreActor << "\n";
        }
        pausar();
    }

    void buscarPorDirector() {
        refrescarPantallaConFondo();
        std::cout << "=== BUSCAR CONTENIDO POR DIRECTOR ===\n";
        std::string nombreDirector = leerLinea("Nombre del director a buscar: ");

        // Búsqueda recursiva en la ListaDoble evaluando el predicado lambda
        Audiovisual** res = catalogo.buscarRecursivo([&nombreDirector](Audiovisual* a) {
            return a != nullptr && a->tieneDirector(nombreDirector);
            });

        refrescarPantallaConFondo();
        if (res && *res) {
            std::cout << "=== RESULTADO ENCONTRADO ===\n\n";
            (*res)->mostrar();
        }
        else {
            std::cout << "No se encontro ningun contenido dirigido por: " << nombreDirector << "\n";
        }
        pausar();
    }



    // ---------------- Busqueda ----------------
    void buscarContenido() {
        refrescarPantallaConFondo();
        std::cout << "=== BUSCAR CONTENIDO ===\n";
        std::cout << "1. Por ID \n";
        std::cout << "2. Por Titulo \n";
        std::cout << "3. Por Actor \n";
        std::cout << "4. Por Director \n";
        int op = leerEntero("Opcion: ");

        if (op == 1) {
            int id = leerEntero("ID a buscar: ");
            Audiovisual** res = catalogo.buscarRecursivo([id](Audiovisual* a) { return a->getId() == id; });
            refrescarPantallaConFondo();
            if (res && *res) (*res)->mostrar();
            else std::cout << "No se encontro el ID.\n";
            pausar();
        }
        else if (op == 2) {
            std::string titulo = leerLinea("Titulo a buscar: ");
            Audiovisual** res = catalogo.buscarRecursivo([&titulo](Audiovisual* a) { return a->getTitulo() == titulo; });
            refrescarPantallaConFondo();
            if (res && *res) (*res)->mostrar();
            else std::cout << "No se encontro el titulo.\n";
            pausar();
        }
        else if (op == 3) {
            buscarPorActor();
        }
        else if (op == 4) {
            buscarPorDirector();
        }
    }

    // ---------------- Ordenamientos --------------
    void ordenarCatalogoMenu() {
        if (catalogo.esVacia()) {
            std::cout << "\nEl catalogo esta vacio.\n";
            pausar();
            return;
        }

        refrescarPantallaConFondo();
        std::cout << "=== ORDENAR CATALOGO ===\n";
        std::cout << "1. HeapSort   -> por Popularidad (descendente)\n";
        std::cout << "2. QuickSort  -> por Rating/Calificacion (descendente)\n";
        std::cout << "3. MergeSort  -> por Anio de Lanzamiento (ascendente)\n";
        std::cout << "4. MergeSort  -> por Titulo (alfabetico)\n";
        int op = leerEntero("Opcion: ");

        std::vector<Audiovisual*> arreglo;
        catalogo.recorrer([&arreglo](Audiovisual* a) { arreglo.push_back(a); });

        switch (op) {
        case 1:
            Ordenamientos::heapSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getPopularidad() > b->getPopularidad();
                });
            break;
        case 2:
            Ordenamientos::quickSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getRating() > b->getRating();
                });
            break;
        case 3:
            Ordenamientos::mergeSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getAnio() < b->getAnio();
                });
            break;
        case 4:
            Ordenamientos::mergeSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getTitulo() < b->getTitulo();
                });
            break;
        default:
            std::cout << "Opcion no valida.\n";
            pausar();
            return;
        }

        refrescarPantallaConFondo();
        std::cout << "=== CATALOGO ORDENADO ===\n\n";
        int i = 1;
        for (Audiovisual* a : arreglo) {
            std::cout << i++ << ". " << a->getTitulo() << " (" << a->getAnio() << ") | "
                << "Rating: " << a->getRating() << "/10 | Pop: " << a->getPopularidad() << "\n";
        }
        pausar();
    }

    // ---------------- Metricas recursivas ----------------
    void mostrarMetricasCategoria() {
        refrescarPantallaConFondo();
        std::cout << "=== METRICAS POR CATEGORIA ===\n\n";
        if (categorias.esVacia()) { std::cout << "No hay categorias registradas.\n"; pausar(); return; }

        categorias.recorrer([this](Categoria* cat) {
            ListaSimple<Audiovisual*> deCategoria;
            catalogo.recorrer([&](Audiovisual* a) {
                if (a->getCategoria() && a->getCategoria()->getId() == cat->getId()) deCategoria.agregarFinal(a);
                });

            double totalMinutos = deCategoria.acumularRecursivo([](Audiovisual* a) { return (double)a->getMinutosTotales(); });
            double totalResenias = deCategoria.acumularRecursivo([](Audiovisual* a) { return (double)a->getResenias().getLongitud(); });

            std::cout << "- " << cat->getNombre() << ": " << deCategoria.getLongitud() << " titulos | "
                << "Minutos totales: " << (int)totalMinutos << " | Resenas totales: " << (int)totalResenias << "\n";
            });
        pausar();
    }

    // ---------------- Administrador ----------------
    void agregarContenido() {
        refrescarPantallaConFondo();
        std::cout << "=== AGREGAR CONTENIDO ===\n";
        std::cout << "1. Pelicula\n2. Serie\n";
        int tipo = leerEntero("Tipo: ");

        int id = leerEntero("ID: ");
        std::string titulo = leerLinea("Titulo: ");
        int anio = leerEntero("Anio: ");
        std::string nomCat = leerLinea("Categoria: ");
        std::string nomDirector = leerLinea("Director: ");
        std::string nomActor = leerLinea("Actor principal: ");

        Categoria** catExistente = categorias.buscarSi([&nomCat](Categoria* c) { return c->getNombre() == nomCat; });
        Categoria* cat = catExistente ? *catExistente : new Categoria(categorias.getLongitud() + 1, nomCat);
        if (!catExistente) categorias.agregarFinal(cat);

        Director** dirExistente = directoresGlobal.buscarSi([&nomDirector](Director* d) { return d->getNombres() == nomDirector; });
        Director* dir = dirExistente ? *dirExistente : new Director(nomDirector, "");
        if (!dirExistente) directoresGlobal.agregarFinal(dir);

        Actor** actExistente = actoresGlobal.buscarSi([&nomActor](Actor* a) { return a->getNombres() == nomActor; });
        Actor* act = actExistente ? *actExistente : new Actor(nomActor, "");
        if (!actExistente) actoresGlobal.agregarFinal(act);

        Audiovisual* nuevo = nullptr;
        if (tipo == 1) {
            int duracion = leerEntero("Duracion (min): ");
            nuevo = new Pelicula(id, titulo, anio, duracion, cat);
        }
        else {
            int temporadas = leerEntero("Numero de temporadas: ");
            int episodios = leerEntero("Numero de episodios: ");
            std::string sEmision = leerLinea("En emision? (s/n): ");
            nuevo = new Serie(id, titulo, anio, temporadas, episodios, (sEmision == "s" || sEmision == "S"), cat);
        }

        nuevo->agregarDirector(dir);
        nuevo->agregarActor(act);

        catalogo.agregarFinal(nuevo);
        guardarTodo();

        std::cout << "\n[OK] Contenido agregado y archivos actualizados.\n";
        pausar();
    }

    // ---------------- Menus ----------------
    void menuCliente() {
        char salir = ' ';
        do {
            refrescarPantallaConFondo();
            std::cout << "========================================\n";
            usuarioLogueado->mostrarMenu();
            std::cout << "========================================\n";
            int opcion = leerEntero("Seleccione una opcion: ");

            switch (opcion) {
            case 1: mostrarCatalogoFicha(); break;
            case 2: mostrarTendencias(); break;
            case 3: buscarContenido(); break;
            case 4: ordenarCatalogoMenu(); break;
            case 5: {
                refrescarPantallaConFondo();
                usuarioLogueado->mostrarPerfil();
                UsuarioCliente* c = dynamic_cast<UsuarioCliente*>(usuarioLogueado);
                if (c) {
                    std::cout << "\n--- MIS RESENAS ---\n";
                    if (c->getMisResenas().esVacia()) std::cout << "Aun no has dejado resenas.\n";
                    else c->getMisResenas().recorrer([](Resenia* r) { r->mostrar(); });
                }
                pausar();
                break;
            }
            case 6:
                salir = 'S';
                break;
            default:
                std::cout << "Opcion no valida.\n";
                pausar();
            }
        } while (salir != 'S');
    }

    void menuAdmin() {
        char salir = ' ';
        do {
            refrescarPantallaConFondo();
            std::cout << "========================================\n";
            usuarioLogueado->mostrarMenu();
            std::cout << "========================================\n";
            int opcion = leerEntero("Seleccione una opcion: ");

            switch (opcion) {
            case 1: agregarContenido(); break;
            case 2: mostrarCatalogoFicha(); break;
            case 3: mostrarTendencias(); break;
            case 4: ordenarCatalogoMenu(); break;
            case 5:
                guardarTodo();
                std::cout << "\n[OK] Todos los archivos fueron actualizados.\n";
                pausar();
                break;
            case 6:
                salir = 'S';
                break;
            default:
                std::cout << "Opcion no valida.\n";
                pausar();
            }
        } while (salir != 'S');
    }

public:
    Sistema() : usuarioLogueado(nullptr) {}

    ~Sistema() {
        guardarTodo();
        catalogo.recorrer([](Audiovisual* item) { delete item; });
        categorias.recorrer([](Categoria* cat) { delete cat; });
        actoresGlobal.recorrer([](Actor* a) { delete a; });
        directoresGlobal.recorrer([](Director* d) { delete d; });
        usuarios.recorrer([](UsuarioBase* usr) { delete usr; });
    }

    void iniciar() {
        inicializarDatos();

        while (true) {
            if (!iniciarSesion()) {
                continue; 
            }

            UsuarioAdministrador* admin = dynamic_cast<UsuarioAdministrador*>(usuarioLogueado);
            if (admin) {
                menuAdmin();
            }
            else {
                menuCliente();
            }

            usuarioLogueado = nullptr;
        }

        refrescarPantallaConFondo();
        std::cout << "Gracias por usar Movie Time.\n";
    }
};