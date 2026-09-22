#pragma once
#include <iostream>
#include <string>
#include "ListaDoble.h"
#include "ListaSimple.h"
#include "Cola.h"
#include "Audiovisual.h"
#include "Pelicula.h"
#include "Serie.h"
#include "Categoria.h"
#include "UsuarioBase.h"
#include "UsuarioCliente.h"
#include "UsuarioAdministrador.h"
#include "GestorArchivo.h"

class Sistema {
private:
    ListaDoble<Audiovisual*> catalogo;
    ListaDoble<Categoria*> categorias;
    Cola<Audiovisual*> colaTendencias;
    ListaSimple<UsuarioBase*> usuarios;
    UsuarioBase* usuarioLogueado;

    void inicializarUsuarios() {
        usuarios.agregarInicio(new UsuarioCliente("1", "Juan", "Perez", "Peru", "1234", "cliente@mail.com", "2000-01-01"));
        usuarios.agregarInicio(new UsuarioAdministrador("2", "Admin", "General", "Peru", "admin123", "admin@mail.com", "1995-05-05", "SuperAdmin"));
    }

    void cargarTendenciasIniciales() {
        NodoDoble<Audiovisual*>* aux = catalogo.getCabeza();
        while (aux != nullptr) {
            // Entran a la cola los títulos con alto rating o popularidad
            if (aux->valor->getRating() >= 4.0 || aux->valor->getPopularidad() > 5) {
                colaTendencias.insertar(aux->valor);
            }
            aux = aux->siguiente;
        }
    }

    bool iniciarSesion() {
        std::string correo, contra;
        std::cout << "\n========================================\n";
        std::cout << "         INICIO DE SESION              \n";
        std::cout << "========================================\n";
        std::cout << "Correo: ";
        std::cin >> correo;
        std::cout << "Contrasena: ";
        std::cin >> contra;

        UsuarioBase** encontrado = usuarios.buscarSi([&correo, &contra](UsuarioBase* u) {
            return u->autenticar(correo, contra);
            });

        if (encontrado != nullptr) {
            usuarioLogueado = *encontrado;
            std::cout << "\n[OK] Bienvenido(a), " << usuarioLogueado->getNombres() << "!\n";
            return true;
        }

        std::cout << "\n[!] Credenciales incorrectas.\n";
        return false;
    }

    void menuCliente() {
        UsuarioCliente* cliente = dynamic_cast<UsuarioCliente*>(usuarioLogueado);
        int opcion = 0;
        do {
            std::cout << "\n----------------------------------------\n";
            usuarioLogueado->mostrarMenu();
            std::cout << "Seleccione una opcion: ";
            std::cin >> opcion;

            switch (opcion) {
            case 1:
                std::cout << "\n--- CATALOGO GENERAL ---\n";
                if (catalogo.esVacia()) {
                    std::cout << "El catalogo esta vacio.\n";
                }
                else {
                    catalogo.recorrer([](Audiovisual* item) {
                        item->mostrar();
                        std::cout << "Rating: " << item->getRating() << " | Popularidad: " << item->getPopularidad() << "\n";
                        std::cout << "----------------------------------------\n";
                        });
                }
                break;

            case 2:
                std::cout << "\n--- TENDENCIAS ACTUALES (COLA) ---\n";
                if (colaTendencias.empty()) {
                    std::cout << "No hay tendencias por mostrar actualmente.\n";
                }
                else {
                    Audiovisual* top = colaTendencias.extraer();
                    std::cout << "Top en Tendencia consumido de la cola:\n";
                    top->mostrar();
                    std::cout << "Rating: " << top->getRating() << " | Popularidad: " << top->getPopularidad() << "\n";
                }
                break;

            case 3: {
                std::cout << "\n--- DEJAR RESEÑA ---\n";
                if (catalogo.esVacia()) {
                    std::cout << "No hay peliculas o series para reseñar.\n";
                    break;
                }

                int idBuscado;
                std::cout << "Ingrese el ID del contenido a reseñar: ";
                std::cin >> idBuscado;

                Audiovisual** buscado = catalogo.buscarSi([idBuscado](Audiovisual* item) {
                    return item->getId() == idBuscado;
                    });

                if (buscado != nullptr) {
                    int estrellas;
                    std::string comentario;

                    std::cout << "Puntuacion (1 al 5): ";
                    std::cin >> estrellas;
                    std::cin.ignore();
                    std::cout << "Comentario: ";
                    std::getline(std::cin, comentario);

                    Resenia* nuevaResenia = new Resenia(estrellas, comentario);

                    // 1. Agregar la reseña al contenido (recalcula el Rating y sube Popularidad)
                    (*buscado)->agregarResenia(nuevaResenia);

                    // 2. Guardar en el historial personal del Cliente
                    if (cliente != nullptr) {
                        cliente->agregarResenia(nuevaResenia);
                    }

                    // 3. Evaluar si ingresa a Tendencia tras el cambio
                    if ((*buscado)->getRating() >= 4.0) {
                        colaTendencias.insertar(*buscado);
                    }

                    // 4. Guardar inmediatamente los datos numericos actualizados en el .txt
                    GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);

                    std::cout << "\n[OK] Reseña agregada correctamente.\n";
                    std::cout << "Nuevo Rating de \"" << (*buscado)->getTitulo() << "\": " << (*buscado)->getRating() << "\n";
                }
                else {
                    std::cout << "\n[!] ID no encontrado.\n";
                }
                break;
            }

            case 4:
                if (cliente != nullptr) {
                    cliente->mostrarMisResenas();
                }
                break;

            case 5:
                std::cout << "\nCerrando sesion...\n";
                break;

            default:
                std::cout << "\nOpcion no valida.\n";
            }
        } while (opcion != 5);
    }

    void menuAdmin() {
        int opcion = 0;
        do {
            std::cout << "\n----------------------------------------\n";
            usuarioLogueado->mostrarMenu();
            std::cout << "Seleccione una opcion: ";
            std::cin >> opcion;

            switch (opcion) {
            case 1: {
                int id, anio, duracion;
                std::string titulo, nomCat;
                std::cout << "\n--- AGREGAR PELICULA ---\n";
                std::cout << "ID: "; std::cin >> id;
                std::cin.ignore();
                std::cout << "Titulo: "; std::getline(std::cin, titulo);
                std::cout << "Anio: "; std::cin >> anio;
                std::cout << "Duracion (min): "; std::cin >> duracion;
                std::cout << "Categoria: "; std::cin >> nomCat;

                Categoria* cat = new Categoria(categorias.getLongitud() + 1, nomCat);
                categorias.agregarFinal(cat);

                Pelicula* p = new Pelicula(id, titulo, anio, duracion, cat);
                catalogo.agregarFinal(p);

                GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);
                std::cout << "[OK] Pelicula agregada y archivo 'catalogo.txt' actualizado.\n";
                break;
            }
            case 2:
                std::cout << "\n--- CATALOGO COMPLETO ---\n";
                if (catalogo.esVacia()) {
                    std::cout << "El catalogo esta vacio.\n";
                }
                else {
                    catalogo.recorrer([](Audiovisual* item) {
                        item->mostrar();
                        std::cout << "Rating: " << item->getRating() << " | Popularidad: " << item->getPopularidad() << "\n";
                        std::cout << "----------------------------------------\n";
                        });
                }
                break;

            case 3:
                std::cout << "\n--- TENDENCIAS ACTUALES (COLA) ---\n";
                if (colaTendencias.empty()) {
                    std::cout << "No hay tendencias registradas.\n";
                }
                else {
                    Audiovisual* top = colaTendencias.extraer();
                    std::cout << "Elemento extraido de la cola de tendencias:\n";
                    top->mostrar();
                }
                break;

            case 4:
                std::cout << "\nGuardando cambios en catalogo.txt...\n";
                GestorArchivos::guardarCatalogo("catalogo.txt", catalogo);
                break;

            case 5:
                std::cout << "\nCerrando sesion de administrador...\n";
                break;

            default:
                std::cout << "\nOpcion no valida.\n";
            }
        } while (opcion != 5);
    }

public:
    Sistema() : usuarioLogueado(nullptr) {}

    ~Sistema() {
        catalogo.recorrer([](Audiovisual* item) { delete item; });
        categorias.recorrer([](Categoria* cat) { delete cat; });
        usuarios.recorrer([](UsuarioBase* usr) { delete usr; });
    }

    void iniciar() {
        GestorArchivos::cargarCatalogo("catalogo.txt", catalogo, categorias);
        cargarTendenciasIniciales();
        inicializarUsuarios();

        if (iniciarSesion()) {
            UsuarioAdministrador* admin = dynamic_cast<UsuarioAdministrador*>(usuarioLogueado);
            if (admin != nullptr) {
                menuAdmin();
            }
            else {
                menuCliente();
            }
        }
    }
};