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
        : UsuarioBase(id, nom, ape, naci, contr, correo, fecha) {}

    std::string getRol() const override { return "CLIENTE"; }

    void agregarResenia(Resenia* r) { misResenas.agregarInicio(r); }
    const ListaSimple<Resenia*>& getMisResenas() const { return misResenas; }


    

    // Recursivo (metrica): total de resenas escritas.
    double getTotalResenias() const {
        return misResenas.acumularRecursivo([](Resenia*) { return 1.0; });
    }

    void mostrarPerfil() override {
        std::cout << "--- PERFIL CLIENTE ---\n"
            << "Nombre: " << nombres << " " << apellidos << "\n"
            << "Correo: " << correo << "\n"
            << "Resenas escritas: " << (int)getTotalResenias() << "\n";
    }

    void mostrarMenu() override {
        std::cout << "1. Ver Catalogo (modo ficha)\n"
            << "2. Ver Tendencias del Dia\n"
            << "3. Buscar por ID / Titulo\n"
            << "4. Ordenar Catalogo\n"
            << "5. Mi Perfil y Mis Resenas\n"
            << "6. Cerrar Sesion\n";
    }
};
