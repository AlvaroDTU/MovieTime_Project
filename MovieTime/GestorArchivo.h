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

class GestorArchivos {
private:
    // Auxiliar para no duplicar categorías al leer el archivo
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

public:
    // --- CARGAR DATOS DESDE EL TXT ---
    static void cargarCatalogo(const std::string& nombreArchivo, ListaDoble<Audiovisual*>& catalogo, ListaDoble<Categoria*>& listaCategorias) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] No se pudo abrir el archivo " << nombreArchivo << ". Se iniciara con catalogo vacio." << std::endl;
            return;
        }

        std::string linea;
        int cargados = 0;

        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;

            std::stringstream ss(linea);
            std::string tipo;
            std::getline(ss, tipo, ',');

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
                Director* dir = new Director(nomDirector, "");
                Actor* act = new Actor(nomActor, "");

                Pelicula* p = new Pelicula(std::stoi(sId), titulo, std::stoi(sAnio), std::stoi(sDuracion), cat);
                p->agregarDirector(dir);
                p->agregarActor(act);
                p->setRating(std::stod(sRating));
                p->setPopularidad(std::stoi(sPop));

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
                Director* dir = new Director(nomDirector, "");
                Actor* act = new Actor(nomActor, "");

                Serie* s = new Serie(std::stoi(sId), titulo, std::stoi(sAnio), std::stoi(sTemp), std::stoi(sEpis), (sEmision == "1"), cat);
                s->agregarDirector(dir);
                s->agregarActor(act);
                s->setRating(std::stod(sRating));
                s->setPopularidad(std::stoi(sPop));

                catalogo.agregarFinal(s);
                cargados++;
            }
        }

        archivo.close();
        std::cout << " [OK] " << cargados << " elementos cargados correctamente desde " << nombreArchivo << std::endl;
    }

    // --- GUARDAR DATOS EN EL TXT ---
    static void guardarCatalogo(const std::string& nombreArchivo, const ListaDoble<Audiovisual*>& catalogo) {
        std::ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "[!] Error al guardar en el archivo " << nombreArchivo << std::endl;
            return;
        }

        catalogo.recorrer([&archivo](Audiovisual* item) {
            std::string nomCat = item->getCategoria() ? item->getCategoria()->getNombre() : "Sin Categoria";

            // Obtener el primer director y actor si existen
            std::string nomDirector = "Desconocido";
            if (!item->getDirectores().esVacia()) {
                nomDirector = item->getDirectores().getCabeza()->valor->getNombres();
            }

            std::string nomActor = "Desconocido";
            if (!item->getActores().esVacia()) {
                nomActor = item->getActores().getCabeza()->valor->getNombres();
            }

            if (item->getTipo() == "Pelicula") {
                Pelicula* p = dynamic_cast<Pelicula*>(item);
                if (p) {
                    archivo << "PELICULA," << p->getId() << "," << p->getTitulo() << ","
                        << p->getAnio() << "," << p->getDuracion() << ","
                        << nomCat << "," << nomDirector << "," << nomActor << ","
                        << p->getRating() << "," << p->getPopularidad() << "\n";
                }
            }
            else if (item->getTipo() == "Serie") {
                Serie* s = dynamic_cast<Serie*>(item);
                if (s) {
                    archivo << "SERIE," << s->getId() << "," << s->getTitulo() << ","
                        << s->getAnio() << "," << s->getNumeroTemporadas() << ","
                        << s->getNumeroEpisodios() << "," << (s->getEnEmision() ? "1" : "0") << ","
                        << nomCat << "," << nomDirector << "," << nomActor << ","
                        << s->getRating() << "," << s->getPopularidad() << "\n";
                }
            }
            });

        archivo.close();
        std::cout << " [OK] Cambios guardados en " << nombreArchivo << std::endl;
    }
};