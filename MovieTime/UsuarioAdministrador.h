#pragma once
#include "UsuarioBase.h"

class UsuarioAdministrador : public UsuarioBase {
private:
    std::string nivelAcceso;

public:
    UsuarioAdministrador(std::string id, std::string nom, std::string ape,
        std::string naci, std::string contr, std::string correo,
        std::string fecha, std::string nivel = "Admin")
        : UsuarioBase(id, nom, ape, naci, contr, correo, fecha), nivelAcceso(nivel) {}

    std::string getRol() const override { return "ADMIN"; }
    std::string getNivelAcceso() const { return nivelAcceso; }

    void mostrarPerfil() override {
        std::cout << "--- PERFIL ADMINISTRADOR ---\n"
            << "Nombre: " << nombres << " " << apellidos << "\n"
            << "Correo: " << correo << "\n"
            << "Nivel de Acceso: " << nivelAcceso << "\n";
    }

    void mostrarMenu() override {
        std::cout << "1. Agregar Pelicula/Serie\n"
            << "2. Ver Catalogo (modo ficha)\n"
            << "3. Ver Tendencias (Cola)\n"
            << "4. Ordenar Catalogo (HeapSort/QuickSort/MergeSort)\n"
            << "5. Guardar Cambios en Archivos\n"
            << "6. Cerrar Sesion\n";
    }
};
