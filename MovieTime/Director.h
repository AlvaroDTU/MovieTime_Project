#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"


class Director : public Persona {
public:
    Director(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {}
    ~Director() {}
};
