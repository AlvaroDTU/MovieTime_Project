#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Audiovisual; 

class Actor : public Persona {
private:
    ListaSimple<Audiovisual*> proyectos;

    void construirRedRecursivo(Nodo<Audiovisual*>* nodoActual, ListaSimple<Audiovisual*>& resultado) const {
        if (nodoActual == nullptr) return; 
        resultado.agregarFinal(nodoActual->valor);
        construirRedRecursivo(nodoActual->siguiente, resultado);
    }

public:
    Actor(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {}
    ~Actor() {}

    void agregarProyecto(Audiovisual* proyecto) {
        proyectos.agregarInicio(proyecto);
    }

    ListaSimple<Audiovisual*> construirRedDeContenidos() const {
        ListaSimple<Audiovisual*> resultado;
        construirRedRecursivo(proyectos.getCabeza(), resultado);
        return resultado;
    }
};
