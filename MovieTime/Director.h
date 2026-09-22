#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Audiovisual; // Declaración adelantada

class Director : public Persona {
private:
    ListaSimple<Audiovisual*> proyectosDirigidos;

public:
    Director(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {
    }
    ~Director() {}

    void agregarProyecto(Audiovisual* proyecto) {
        proyectosDirigidos.agregarInicio(proyecto);
    }

    ListaSimple<Audiovisual*>& getProyectosDirigidos() { return proyectosDirigidos; }
};