#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ListaDoble.h"
#include "ListaSimple.h"
#include "Audiovisual.h"
#include "Pelicula.h"
#include "Serie.h"
#include "Categoria.h"
#include "Director.h"
#include "Actor.h"
#include "Resenia.h"
#include "UsuarioBase.h"
#include "UsuarioCliente.h"
#include "UsuarioAdministrador.h"

class GestorArchivos {
private:
    static Categoria* obtenerOCrearCategoria(const std::string& nombreCat, ListaDoble<Categoria*>& listaCategorias) {
        Categoria** existente = listaCategorias.buscarSi([&nombreCat](Categoria* c) {
            return c->getNombre() == nombreCat;
            });
        if (existente != nullptr) return *existente;

        int nuevoId = listaCategorias.getLongitud() + 1;
        Categoria* nuevaCat = new Categoria(nuevoId, nombreCat);
        listaCategorias.agregarFinal(nuevaCat);
        return nuevaCat;
    }

    // Reemplaza comas por punto y coma para no romper el formato CSV simple.
    static std::string sanear(std::string texto) {
        for (auto& c : texto) if (c == ',') c = ';';
        return texto;
    }

public:
    // ================= CATALOGO (peliculas.txt / catalogo.txt) =================
    static void cargarCatalogo(const std::string& nombreArchivo, ListaDoble<Audiovisual*>& catalogo,
        ListaDoble<Categoria*>& listaCategorias, ListaDoble<Actor*>& listaActores,
        ListaDoble<Director*>& listaDirectores) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] No se pudo abrir " << nombreArchivo << ". Se inicia con catalogo vacio.\n";
            return;
        }

        std::string linea;
        int cargados = 0;

        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;
            std::stringstream ss(linea);
            std::string tipo;
            std::getline(ss, tipo, ',');

            auto obtenerActor = [&](const std::string& nombre) -> Actor* {
                Actor** existente = listaActores.buscarSi([&nombre](Actor* a) { return a->getNombres() == nombre; });
                if (existente) return *existente;
                Actor* nuevo = new Actor(nombre, "");
                listaActores.agregarFinal(nuevo);
                return nuevo;
                };
            auto obtenerDirector = [&](const std::string& nombre) -> Director* {
                Director** existente = listaDirectores.buscarSi([&nombre](Director* d) { return d->getNombres() == nombre; });
                if (existente) return *existente;
                Director* nuevo = new Director(nombre, "");
                listaDirectores.agregarFinal(nuevo);
                return nuevo;
                };

            if (tipo == "PELICULA") {
                std::string sId, titulo, sAnio, sDuracion, nomCat, nomDirector, nomActor, sRating, sPop;
                std::getline(ss, sId, ',');
                std::getline(ss, titulo, ',');
                std::getline(ss, sAnio, ',');
                std::getline(ss, sDuracion, ',');
                std::getline(ss, nomCat, ',');
                std::getline(ss, nomDirector, ',');
                std::getline(ss, nomActor, ',');
                std::getline(ss, sRating, ',');
                std::getline(ss, sPop, ',');

                Categoria* cat = obtenerOCrearCategoria(nomCat, listaCategorias);
                Director* dir = obtenerDirector(nomDirector);
                Actor* act = obtenerActor(nomActor);

                Pelicula* p = new Pelicula(std::stoi(sId), titulo, std::stoi(sAnio), std::stoi(sDuracion), cat);
                p->agregarDirector(dir);
                p->agregarActor(act);
                p->setRating(std::stod(sRating));
                p->setPopularidad(std::stoi(sPop));

                dir->agregarProyecto(p);
                act->agregarProyecto(p);

                catalogo.agregarFinal(p);
                cargados++;
            }
            else if (tipo == "SERIE") {
                std::string sId, titulo, sAnio, sTemp, sEpis, sEmision, nomCat, nomDirector, nomActor, sRating, sPop;
                std::getline(ss, sId, ',');
                std::getline(ss, titulo, ',');
                std::getline(ss, sAnio, ',');
                std::getline(ss, sTemp, ',');
                std::getline(ss, sEpis, ',');
                std::getline(ss, sEmision, ',');
                std::getline(ss, nomCat, ',');
                std::getline(ss, nomDirector, ',');
                std::getline(ss, nomActor, ',');
                std::getline(ss, sRating, ',');
                std::getline(ss, sPop, ',');

                Categoria* cat = obtenerOCrearCategoria(nomCat, listaCategorias);
                Director* dir = obtenerDirector(nomDirector);
                Actor* act = obtenerActor(nomActor);

                Serie* s = new Serie(std::stoi(sId), titulo, std::stoi(sAnio), std::stoi(sTemp),
                    std::stoi(sEpis), (sEmision == "1"), cat);
                s->agregarDirector(dir);
                s->agregarActor(act);
                s->setRating(std::stod(sRating));
                s->setPopularidad(std::stoi(sPop));

                dir->agregarProyecto(s);
                act->agregarProyecto(s);

                catalogo.agregarFinal(s);
                cargados++;
            }
        }
        archivo.close();
        std::cout << "[OK] " << cargados << " elementos cargados desde " << nombreArchivo << "\n";
    }

    static void guardarCatalogo(const std::string& nombreArchivo, const ListaDoble<Audiovisual*>& catalogo) {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] Error al guardar en " << nombreArchivo << "\n";
            return;
        }

        catalogo.recorrer([&archivo](Audiovisual* item) {
            std::string nomCat = item->getCategoria() ? item->getCategoria()->getNombre() : "Sin Categoria";

            std::string nomDirector = "Desconocido";
            if (!item->getDirectores().esVacia()) nomDirector = item->getDirectores().getCabeza()->valor->getNombres();

            std::string nomActor = "Desconocido";
            if (!item->getActores().esVacia()) nomActor = item->getActores().getCabeza()->valor->getNombres();

            if (item->getTipo() == "Pelicula") {
                Pelicula* p = dynamic_cast<Pelicula*>(item);
                if (p) {
                    archivo << "PELICULA," << p->getId() << "," << sanear(p->getTitulo()) << ","
                        << p->getAnio() << "," << p->getDuracion() << ","
                        << sanear(nomCat) << "," << sanear(nomDirector) << "," << sanear(nomActor) << ","
                        << p->getRating() << "," << p->getPopularidad() << "\n";
                }
            }
            else if (item->getTipo() == "Serie") {
                Serie* s = dynamic_cast<Serie*>(item);
                if (s) {
                    archivo << "SERIE," << s->getId() << "," << sanear(s->getTitulo()) << ","
                        << s->getAnio() << "," << s->getNumeroTemporadas() << ","
                        << s->getNumeroEpisodios() << "," << (s->getEnEmision() ? "1" : "0") << ","
                        << sanear(nomCat) << "," << sanear(nomDirector) << "," << sanear(nomActor) << ","
                        << s->getRating() << "," << s->getPopularidad() << "\n";
                }
            }
            });
        archivo.close();
        std::cout << "[OK] Catalogo guardado en " << nombreArchivo << "\n";
    }

    // ================= PERSONAS (personas.txt) =================
    static void guardarPersonas(const std::string& nombreArchivo, const ListaDoble<Actor*>& actores,
        const ListaDoble<Director*>& directores) {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) { std::cout << "[!] Error al guardar " << nombreArchivo << "\n"; return; }

        actores.recorrer([&archivo](Actor* a) {
            archivo << "ACTOR," << sanear(a->getNombres()) << "," << sanear(a->getApellidos()) << "\n";
            });
        directores.recorrer([&archivo](Director* d) {
            archivo << "DIRECTOR," << sanear(d->getNombres()) << "," << sanear(d->getApellidos()) << "\n";
            });
        archivo.close();
        std::cout << "[OK] Reparto guardado en " << nombreArchivo << "\n";
    }

    // ================= RESEnAS (resenias.txt) =================
    static void guardarResenias(const std::string& nombreArchivo, const ListaDoble<Audiovisual*>& catalogo) {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) { std::cout << "[!] Error al guardar " << nombreArchivo << "\n"; return; }

        catalogo.recorrer([&archivo](Audiovisual* item) {
            item->getResenias().recorrer([&archivo, item](Resenia* r) {
                archivo << item->getId() << "," << r->getIdUsuario() << ","
                    << r->getCalificacion() << "," << sanear(r->getComentario()) << "\n";
                });
            });
        archivo.close();
        std::cout << "[OK] Resenias guardadas en " << nombreArchivo << "\n";
    }

    static void cargarResenias(const std::string& nombreArchivo, ListaDoble<Audiovisual*>& catalogo,
        ListaSimple<UsuarioBase*>& usuarios) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] No se encontro " << nombreArchivo << ". Se inicia sin resenias previas.\n";
            return;
        }

        std::string linea;
        int cargadas = 0;
        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;
            std::stringstream ss(linea);
            std::string sIdContenido, idUsuario, sCalif, comentario;
            std::getline(ss, sIdContenido, ',');
            std::getline(ss, idUsuario, ',');
            std::getline(ss, sCalif, ',');
            std::getline(ss, comentario, ',');

            int idContenido = std::stoi(sIdContenido);
            double calif = std::stod(sCalif);

            Audiovisual** contenido = catalogo.buscarSi([idContenido](Audiovisual* a) { return a->getId() == idContenido; });
            if (!contenido) continue;

            Resenia* r = new Resenia(idContenido, idUsuario, calif, comentario);
            (*contenido)->agregarResenia(r);

            UsuarioBase** usuario = usuarios.buscarSi([&idUsuario](UsuarioBase* u) { return u->getId() == idUsuario; });
            if (usuario) {
                UsuarioCliente* cliente = dynamic_cast<UsuarioCliente*>(*usuario);
                if (cliente) cliente->agregarResenia(r);
            }
            cargadas++;
        }
        archivo.close();
        std::cout << "[OK] " << cargadas << " resenias cargadas desde " << nombreArchivo << "\n";
    }

    // ================= USUARIOS (usuarios.txt) =================
    static void guardarUsuarios(const std::string& nombreArchivo, const ListaSimple<UsuarioBase*>& usuarios) {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) { std::cout << "[!] Error al guardar " << nombreArchivo << "\n"; return; }

        usuarios.recorrer([&archivo](UsuarioBase* u) {
            archivo << u->getRol() << "," << u->getId() << "," << sanear(u->getNombres()) << ","
                << sanear(u->getApellidos()) << "," << sanear(u->getNacionalidad()) << ","
                << u->getContrasenia() << "," << u->getCorreo() << "," << u->getFechaNacimiento() << "\n";
            });
        archivo.close();
        std::cout << "[OK] Usuarios guardados en " << nombreArchivo << "\n";
    }

    static void cargarUsuarios(const std::string& nombreArchivo, ListaSimple<UsuarioBase*>& usuarios) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] No se encontro " << nombreArchivo << ". Se crearan usuarios por defecto.\n";
            usuarios.agregarInicio(new UsuarioCliente("1", "Juan", "Perez", "Peru", "1234", "cliente@mail.com", "2000-01-01"));
            usuarios.agregarInicio(new UsuarioAdministrador("2", "Admin", "General", "Peru", "admin123", "admin@mail.com", "1995-05-05", "SuperAdmin"));
            return;
        }

        std::string linea;
        int cargados = 0;
        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;
            std::stringstream ss(linea);
            std::string rol, id, nom, ape, naci, contr, correo, fecha;
            std::getline(ss, rol, ',');
            std::getline(ss, id, ',');
            std::getline(ss, nom, ',');
            std::getline(ss, ape, ',');
            std::getline(ss, naci, ',');
            std::getline(ss, contr, ',');
            std::getline(ss, correo, ',');
            std::getline(ss, fecha, ',');

            if (rol == "ADMIN") {
                usuarios.agregarFinal(new UsuarioAdministrador(id, nom, ape, naci, contr, correo, fecha));
            }
            else {
                usuarios.agregarFinal(new UsuarioCliente(id, nom, ape, naci, contr, correo, fecha));
            }
            cargados++;
        }
        archivo.close();
        std::cout << "[OK] " << cargados << " usuarios cargados desde " << nombreArchivo << "\n";
    }
};
