#pragma once
#include <iostream>
#include "Audiovisual.h"

class Pelicula : public Audiovisual {
private:
    int duracionMinutos;

public:

    Pelicula(int id, const std::string& titulo, int anio, int duracionMinutos, Categoria* categoria)
        : Audiovisual(id, titulo, anio, categoria), duracionMinutos(duracionMinutos) {
    }

    virtual ~Pelicula() override {}

    int getDuracion() const { return duracionMinutos; }
    void setDuracion(int d) { duracionMinutos = d; }

    std::string getTipo() const override { return "Pelicula"; }

    void mostrar() const override {
        std::cout << "[PELÍCULA] ID: " << id << " | " << titulo << " (" << anio << ")\n"
            << "  Duración: " << duracionMinutos << " min\n"
            << "  Categoría: " << (categoria ? categoria->getNombre() : "Sin Categoria") << "\n"
            << "  Rating: " << rating << " | Popularidad: " << popularidad << "\n";
    }
};