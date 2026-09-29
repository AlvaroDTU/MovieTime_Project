#pragma once
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

class Sistema {
private:
    ListaDoble<Audiovisual*> catalogo;
    ListaDoble<Categoria*> categorias;
    ListaDoble<Actor*> actoresGlobal;
    ListaDoble<Director*> directoresGlobal;
    Cola<Audiovisual*> colaTendencias;
    ListaSimple<UsuarioBase*> usuarios;
    UsuarioBase* usuarioLogueado;

    static const int MIN_RATING_TENDENCIA = 8;   // rating >= 8/10
    static const int MIN_POPULARIDAD_TENDENCIA = 5;

    static void limpiarPantalla() {
        system("cls");
    }

    static void pausar() {
        std::cout << "\nPresione ENTER para continuar...";
        std::cin.get();
    }

    static int leerEntero(const std::string& mensaje) {
        int valor;
        std::cout << mensaje;
        while (!(std::cin >> valor)) {
            if (std::cin.eof()) { std::exit(0); } // fin de flujo: cierre limpio, evita bucle infinito
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. " << mensaje;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return valor;
    }

    static double leerDouble(const std::string& mensaje) {
        double valor;
        std::cout << mensaje;
        while (!(std::cin >> valor)) {
            if (std::cin.eof()) { std::exit(0); }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. " << mensaje;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

    // Reconstruye la cola de tendencias: entra todo lo que tenga rating >= 8
    // o popularidad alta y que aun no este en la cola.
    void actualizarTendencias() {
        catalogo.recorrer([this](Audiovisual* item) {
            bool califica = (item->getRating() >= MIN_RATING_TENDENCIA) ||
                (item->getPopularidad() > MIN_POPULARIDAD_TENDENCIA);
            bool yaEsta = colaTendencias.contiene([item](Audiovisual* a) { return a->getId() == item->getId(); });
            if (califica && !yaEsta) colaTendencias.insertar(item);
            });
    }

    // ---------------- Sesion ----------------
    bool iniciarSesion() {
        limpiarPantalla();
        std::string correo, contra;
        std::cout << "========================================\n";
        std::cout << "     SISTEMA DE STREAMING - MOVIE TIME    \n";
        std::cout << "========================================\n";
        std::cout << "Correo: "; std::cin >> correo;
        std::cout << "Contrasena: "; std::cin >> contra;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            limpiarPantalla();
            std::cout << "El catalogo esta vacio.\n";
            pausar();
            return;
        }

        std::vector<Audiovisual*> vista;
        catalogo.recorrer([&vista](Audiovisual* a) { vista.push_back(a); });

        size_t indice = 0;
        char opcion = ' ';

        do {
            limpiarPantalla();
            Audiovisual* actual = vista[indice];

            std::cout << "==================== FICHA " << (indice + 1) << "/" << vista.size() << " ====================\n\n";
            actual->mostrar();
            std::cout << "\nDirector(es): ";
            actual->getDirectores().recorrer([](Director* d) { std::cout << d->getNombreCompleto() << "  "; });
            std::cout << "\nActor(es): ";
            actual->getActores().recorrer([](Actor* a) { std::cout << a->getNombreCompleto() << "  "; });
            std::cout << "\nReseñas registradas: " << actual->getResenias().getLongitud() << "\n";

            std::cout << "\n----------------------------------------\n";
            std::cout << "[N] Siguiente   [P] Anterior   [R] Dejar Reseña   [V] Ver Reseñas   [Q] Volver\n";
            std::cout << "Opcion: ";
            std::cin >> opcion;
            opcion = toupper(opcion);

            switch (opcion) {
            case 'N':
                indice = (indice + 1) % vista.size();
                break;
            case 'P':
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
        limpiarPantalla();
        std::cout << "=== RESEÑAS DE \"" << item->getTitulo() << "\" ===\n\n";
        if (item->getResenias().esVacia()) {
            std::cout << "Aun no hay reseñas para este contenido.\n";
        }
        else {
            item->getResenias().recorrer([](Resenia* r) { r->mostrar(); });
        }
        pausar();
    }

    void dejarResenia(Audiovisual* item) {
        UsuarioCliente* cliente = static_cast<UsuarioCliente*>(usuarioLogueado);
        if (!cliente) {
            std::cout << "\nSolo los usuarios pueden dejar reseñas.\n";
            pausar();
            return;
        }

        limpiarPantalla();
        std::cout << "=== DEJAR RESEÑA: \"" << item->getTitulo() << "\" ===\n\n";
        double calif = leerDouble("Calificacion (0 al 10): ");
        std::string comentario = leerLinea("Comentario: ");

        Resenia* nueva = new Resenia(item->getId(), cliente->getId(), calif, comentario);
        item->agregarResenia(nueva);
        cliente->agregarResenia(nueva);
        cliente->registrarReproduccion(item->getMinutosTotales());

        actualizarTendencias();
        GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);
        GestorArchivos::guardarResenias("resenias.txt", catalogo);

        std::cout << "\n[OK] Reseña agregada. Nuevo rating: " << item->getRating() << "/10\n";
        pausar();
    }

    // ---------------- Tendencias ----------------
    void mostrarTendencias() {
        limpiarPantalla();
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

    // ---------------- Busqueda ----------------
    void buscarContenido() {
        limpiarPantalla();
        std::cout << "=== BUSCAR CONTENIDO ===\n";
        std::cout << "1. Por ID (busqueda recursiva en lista enlazada)\n";
        std::cout << "2. Por Titulo (busqueda recursiva en lista enlazada)\n";
        int op = leerEntero("Opcion: ");

        Audiovisual* encontrado = nullptr;

        if (op == 1) {
            int id = leerEntero("ID a buscar: ");
            Audiovisual** res = catalogo.buscarRecursivo([id](Audiovisual* a) { return a->getId() == id; });
            if (res) encontrado = *res;
        }
        else {
            std::string titulo = leerLinea("Titulo a buscar: ");
            Audiovisual** res = catalogo.buscarRecursivo([&titulo](Audiovisual* a) { return a->getTitulo() == titulo; });
            if (res) encontrado = *res;
        }

        limpiarPantalla();
        if (encontrado) {
            std::cout << "=== RESULTADO ===\n\n";
            encontrado->mostrar();
        }
        else {
            std::cout << "No se encontro ningun contenido con ese criterio.\n";
        }
        pausar();
    }

    // ---------------- Ordenamiento (3 algoritmos avanzados) ----------------
    void ordenarCatalogoMenu() {
        if (catalogo.esVacia()) {
            std::cout << "\nEl catalogo esta vacio.\n";
            pausar();
            return;
        }

        limpiarPantalla();
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
                return a->getPopularidad() > b->getPopularidad(); // mayor primero
                });
            break;
        case 2:
            Ordenamientos::quickSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getRating() > b->getRating(); // mayor primero
                });
            break;
        case 3:
            Ordenamientos::mergeSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getAnio() < b->getAnio(); // ascendente
                });
            break;
        case 4:
            Ordenamientos::mergeSort<Audiovisual*>(arreglo, [](Audiovisual* a, Audiovisual* b) {
                return a->getTitulo() < b->getTitulo(); // alfabetico
                });
            break;
        default:
            std::cout << "Opcion no valida.\n";
            pausar();
            return;
        }

        // Se reconstruye el orden de la ListaDoble a partir del arreglo ya ordenado
        catalogo.limpiar();
        for (Audiovisual* a : arreglo) catalogo.agregarFinal(a);

        limpiarPantalla();
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
        limpiarPantalla();
        std::cout << "=== METRICAS POR CATEGORIA ===\n\n";
        if (categorias.esVacia()) { std::cout << "No hay categorias registradas.\n"; pausar(); return; }

        categorias.recorrer([this](Categoria* cat) {
            ListaSimple<Audiovisual*> deCategoria;
            catalogo.recorrer([&](Audiovisual* a) {
                if (a->getCategoria() && a->getCategoria()->getId() == cat->getId()) deCategoria.agregarFinal(a);
                });

            // Recursivo: total de minutos y total de reseñas de la categoria
            double totalMinutos = deCategoria.acumularRecursivo([](Audiovisual* a) { return (double)a->getMinutosTotales(); });
            double totalResenias = deCategoria.acumularRecursivo([](Audiovisual* a) { return (double)a->getResenias().getLongitud(); });

            std::cout << "- " << cat->getNombre() << ": " << deCategoria.getLongitud() << " titulos | "
                << "Minutos totales: " << (int)totalMinutos << " | Reseñas totales: " << (int)totalResenias << "\n";
            });
        pausar();
    }

    // ---------------- Admin: agregar contenido ----------------
    void agregarContenido() {
        limpiarPantalla();
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
        dir->agregarProyecto(nuevo);
        act->agregarProyecto(nuevo);

        catalogo.agregarFinal(nuevo);
        guardarTodo();

        std::cout << "\n[OK] Contenido agregado y archivos actualizados.\n";
        pausar();
    }

    // ---------------- Menus por rol ----------------
    void menuCliente() {
        char salir = ' ';
        do {
            limpiarPantalla();
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
                limpiarPantalla();
                usuarioLogueado->mostrarPerfil();
                UsuarioCliente* c = dynamic_cast<UsuarioCliente*>(usuarioLogueado);
                if (c) {
                    std::cout << "\n--- MIS RESEÑAS ---\n";
                    if (c->getMisResenas().esVacia()) std::cout << "Aun no has dejado reseñas.\n";
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
            limpiarPantalla();
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

    // Bucle principal ininterrumpido: tras cerrar sesion se vuelve al login,
    // nunca se cierra el programa de forma inesperada.
    void iniciar() {
        inicializarDatos();

        while (true) {
            if (!iniciarSesion()) {
                limpiarPantalla();
                std::cout << "1. Reintentar inicio de sesion\n2. Salir del sistema\n";
                int op = leerEntero("Opcion: ");
                if (op == 2) break;
                else continue;
            }

            UsuarioAdministrador* admin = dynamic_cast<UsuarioAdministrador*>(usuarioLogueado);
            if (admin) menuAdmin();
            else menuCliente();

            usuarioLogueado = nullptr; // cierre de sesion: vuelve al login sin cerrar el programa
        }

        limpiarPantalla();
        std::cout << "Gracias por usar Movie Time. Todos los cambios fueron guardados.\n";
    }
};
