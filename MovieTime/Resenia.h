#pragma once
#include <string>
#include <iostream>

class Resenia {
private:
    int idContenido;
    std::string idUsuario;
    double calificacion; // Escala sobre 10 estrellas
    std::string comentario;

public:
    Resenia(int idContenido, const std::string& idUsuario, double calificacion, const std::string& comentario)
        : idContenido(idContenido), idUsuario(idUsuario), calificacion(calificacion), comentario(comentario) {}

    int getIdContenido() const { return idContenido; }
    std::string getIdUsuario() const { return idUsuario; }
    double getCalificacion() const { return calificacion; } // sobre 10
    std::string getComentario() const { return comentario; }

    void mostrar() const {
        std::cout << "  [Usuario " << idUsuario << " | Contenido #" << idContenido << "] "
            << "Calificacion: " << calificacion << "/10\n"
            << "  \"" << comentario << "\"\n";
    }
};
