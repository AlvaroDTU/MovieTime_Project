#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Audiovisual; // Declaracion adelantada

class Director : public Persona {
private:
    ListaSimple<Audiovisual*> proyectosDirigidos;

    void construirRedRecursivo(Nodo<Audiovisual*>* nodoActual, ListaSimple<Audiovisual*>& resultado) const {
        if (nodoActual == nullptr) return;
        resultado.agregarFinal(nodoActual->valor);
        construirRedRecursivo(nodoActual->siguiente, resultado);
    }

public:
    Director(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {}
    ~Director() {}

    void agregarProyecto(Audiovisual* proyecto) {
        proyectosDirigidos.agregarInicio(proyecto);
    }

    ListaSimple<Audiovisual*>& getProyectosDirigidos() { return proyectosDirigidos; }

    ListaSimple<Audiovisual*> construirRedDeContenidos() const {
        ListaSimple<Audiovisual*> resultado;
        construirRedRecursivo(proyectosDirigidos.getCabeza(), resultado);
        return resultado;
    }
};
