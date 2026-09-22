#pragma once
#include "UsuarioBase.h"
#include "Resenia.h"
#include "ListaSimple.h"

class UsuarioCliente : public UsuarioBase {
private:
    ListaSimple<Resenia*> misResenas;

public:
    UsuarioCliente(std::string id, std::string nom, std::string ape, std::string naci,
        std::string contr, std::string correo, std::string fecha)
        : UsuarioBase(id, nom, ape, naci, contr, correo, fecha) {
    }

    void agregarResenia(Resenia* r) {
        misResenas.agregarInicio(r);
    }

    void mostrarMisResenas() {
        std::cout << "\n=== MIS RESEÑAS ESCRITAS ===\n";
        if (misResenas.esVacia()) {
            std::cout << "Aún no has dejado ninguna reseña.\n";
            return;
        }

        misResenas.recorrer([](Resenia* r) {
            std::cout << "Estrellas: " << r->getEstrellas() << "/5\n"
                << "Comentario: " << r->getComentario() << "\n"
                << "----------------------------------------\n";
            });
    }

    void mostrarPerfil() override {
        std::cout << "--- PERFIL CLIENTE ---\n"
            << "Nombre: " << nombres << " " << apellidos << "\n"
            << "Correo: " << correo << "\n"
            << "Reseñas escritas: " << misResenas.getLongitud() << "\n";
    }

    void mostrarMenu() override {
        std::cout << "1. Ver Catálogo\n"
            << "2. Ver Tendencias\n"
            << "3. Dejar Reseña\n"
            << "4. Ver Mis Reseñas\n"
            << "5. Salir\n";
    }
};