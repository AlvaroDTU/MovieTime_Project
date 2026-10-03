#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Actor : public Persona 
{
public:
    Actor(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {}
    ~Actor() {}
};
