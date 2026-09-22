#pragma once
#include <string>

class Persona {
protected:
    std::string nombres;
    std::string apellidos;

public:
    Persona(const std::string& nombres, const std::string& apellidos)
        : nombres(nombres), apellidos(apellidos) {
    }
    virtual ~Persona() {}

    std::string getNombres() const { return nombres; }
    std::string getApellidos() const { return apellidos; }
    std::string getNombreCompleto() const { return nombres + " " + apellidos; }
};