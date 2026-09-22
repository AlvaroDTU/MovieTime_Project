#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Audiovisual; // Declaración adelantada

class Actor : public Persona {
private:
    ListaSimple<Audiovisual*> proyectos;

public:
    Actor(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {
    }
    ~Actor() {}

    void agregarProyecto(Audiovisual* proyecto) {
        proyectos.agregarInicio(proyecto);
    }

    ListaSimple<Audiovisual*>& getProyectos() { return proyectos; }
};