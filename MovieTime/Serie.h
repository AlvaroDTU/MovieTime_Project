#pragma once
#include <iostream>
#include <string>
#include "Audiovisual.h"

class Serie : public Audiovisual {
private:
    int numeroTemporadas;
    int numeroEpisodios;
    bool enEmision;

public:
    Serie(int id, const std::string& titulo, int anio,
        int numeroTemporadas, int numeroEpisodios, bool enEmision, Categoria* categoria)
        : Audiovisual(id, titulo, anio, categoria),
        numeroTemporadas(numeroTemporadas),
        numeroEpisodios(numeroEpisodios),
        enEmision(enEmision) {
    }

    virtual ~Serie() override {}

    int getNumeroTemporadas() const { return numeroTemporadas; }
    int getNumeroEpisodios() const { return numeroEpisodios; }
    bool getEnEmision() const { return enEmision; }

    void setNumeroTemporadas(int n) { numeroTemporadas = n; }
    void setNumeroEpisodios(int n) { numeroEpisodios = n; }
    void setEnEmision(bool e) { enEmision = e; }

    std::string getTipo() const override { return "Serie"; }

    void mostrar() const override {
        std::cout << "[SERIE] ID: " << id << " | " << titulo << " (" << anio << ")\n"
            << "  Temporadas: " << numeroTemporadas << " | Episodios: " << numeroEpisodios << "\n"
            << "  Estado: " << (enEmision ? "En emisión" : "Finalizada") << "\n"
            << "  Categoría: " << (categoria ? categoria->getNombre() : "Sin Categoría") << "\n"
            << "  Rating: " << rating << " | Popularidad: " << popularidad << "\n";
    }
};