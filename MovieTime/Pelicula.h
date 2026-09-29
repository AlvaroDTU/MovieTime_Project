#pragma once
#include <iostream>
#include <string>
#include "Audiovisual.h"

class Pelicula : public Audiovisual {
private:
    int duracionMinutos;

public:
    Pelicula(int id, const std::string& titulo, int anio, int duracionMinutos, Categoria* categoria)
        : Audiovisual(id, titulo, anio, categoria), duracionMinutos(duracionMinutos) {}

    virtual ~Pelicula() override {}

    int getDuracion() const { return duracionMinutos; }
    void setDuracion(int d) { duracionMinutos = d; }

    std::string getTipo() const override { return "Pelicula"; }

    int getMinutosTotales() const override { return duracionMinutos; }

    void mostrar() const override {
        std::cout << "[PELICULA] ID: " << id << " | " << titulo << " (" << anio << ")\n"
            << "  Duracion: " << duracionMinutos << " min\n"
            << "  Categoria: " << (categoria ? categoria->getNombre() : "Sin Categoria") << "\n"
            << "  Rating: " << rating << "/10 | Popularidad: " << popularidad << "\n";
    }
};
