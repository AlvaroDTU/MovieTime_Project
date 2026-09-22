#pragma once
#include <iostream>
#include "Audiovisual.h"

class Pelicula : public Audiovisual {
private:
    int duracionMinutos;

public:
    Pelicula(int id,std::string& titulo, int anio, int duracionMinutos, Categoria* categoria)
        : Audiovisual(id, titulo, anio, categoria),
        duracionMinutos(duracionMinutos) {}

    virtual ~Pelicula() {}

    int getDuracion() const { return duracionMinutos; }
    void setDuracion(int d) { duracionMinutos = d; }

    std::string getTipo() const override { return "Pelicula"; }

    void mostrar() const override {
        std::cout << "Pelicula [" << id << "] " << titulo
            << " (" << anio << ")\n";
        std::cout << "  Duracion: " << duracionMinutos << " min\n";
        std::cout << "  Categoria: " << categoria->getNombre() << "\n";
        std::cout << "  Rating: " << rating
            << " | Popularidad: " << popularidad << "\n";
    }
};

