#pragma once
#include <iostream>
#include <string>

class Categoria {
private:
    int id;
    std::string nombre;

public:
    Categoria(int id, const std::string& nombre)
        : id(id), nombre(nombre) {
    }

    ~Categoria() {}

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }

    void setId(int i) { id = i; }
    void setNombre(const std::string& n) { nombre = n; }

    void mostrar() const {
        std::cout << "Categoria [" << id << "] " << nombre << "\n";
    }
};
