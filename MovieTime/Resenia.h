#pragma once
#include <string>
#include <iostream>

class Resenia {
private:
    int estrellas;
    std::string comentario;

public:
    Resenia(int estrellas, const std::string& comentario)
        : estrellas(estrellas), comentario(comentario) {
    }

    int getEstrellas() const { return estrellas; }
    std::string getComentario() const { return comentario; }

    void mostrar() const {
        std::cout << "  [Puntuación: " << estrellas << "/5] " << comentario << "\n";
    }
};