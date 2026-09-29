#pragma once
#include <iostream>
#include <string>
#include "Audiovisual.h"

class Serie : public Audiovisual {
private:
    int numeroTemporadas;
    int numeroEpisodios;
    bool enEmision;
    int minutosPorEpisodio;

public:
    Serie(int id, const std::string& titulo, int anio,
        int numeroTemporadas, int numeroEpisodios, bool enEmision, Categoria* categoria,
        int minutosPorEpisodio = 45)
        : Audiovisual(id, titulo, anio, categoria),
        numeroTemporadas(numeroTemporadas),
        numeroEpisodios(numeroEpisodios),
        enEmision(enEmision),
        minutosPorEpisodio(minutosPorEpisodio) {}

    virtual ~Serie() override {}

    int getNumeroTemporadas() const { return numeroTemporadas; }
    int getNumeroEpisodios() const { return numeroEpisodios; }
    bool getEnEmision() const { return enEmision; }

    void setNumeroTemporadas(int n) { numeroTemporadas = n; }
    void setNumeroEpisodios(int n) { numeroEpisodios = n; }
    void setEnEmision(bool e) { enEmision = e; }

    std::string getTipo() const override { return "Serie"; }

    int getMinutosTotales() const override { return numeroEpisodios * minutosPorEpisodio; }

    void mostrar() const override {
        std::cout << "[SERIE] ID: " << id << " | " << titulo << " (" << anio << ")\n"
            << "  Temporadas: " << numeroTemporadas << " | Episodios: " << numeroEpisodios << "\n"
            << "  Estado: " << (enEmision ? "En emision" : "Finalizada") << "\n"
            << "  Categoria: " << (categoria ? categoria->getNombre() : "Sin Categoria") << "\n"
            << "  Rating: " << rating << "/10 | Popularidad: " << popularidad << "\n";
    }
};
