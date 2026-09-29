#pragma once
#include <string>
#include "Persona.h"
#include "ListaSimple.h"

class Audiovisual; // Declaracion adelantada

class Actor : public Persona {
private:
    ListaSimple<Audiovisual*> proyectos;

    // --- Recursivo (usa el nodo interno de ListaSimple) ---
    // Construye recursivamente, en "resultado", la red de contenidos
    // en los que participo el actor (requisito 2.1 de la rubrica).
    void construirRedRecursivo(Nodo<Audiovisual*>* nodoActual, ListaSimple<Audiovisual*>& resultado) const {
        if (nodoActual == nullptr) return;              // Caso base
        resultado.agregarFinal(nodoActual->valor);       // Procesa el nodo actual
        construirRedRecursivo(nodoActual->siguiente, resultado); // Avanza recursivamente
    }

public:
    Actor(const std::string& nombres, const std::string& apellidos)
        : Persona(nombres, apellidos) {}
    ~Actor() {}

    void agregarProyecto(Audiovisual* proyecto) {
        proyectos.agregarInicio(proyecto);
    }


    // Punto de entrada publico del algoritmo recursivo de la red de contenidos.
    ListaSimple<Audiovisual*> construirRedDeContenidos() const {
        ListaSimple<Audiovisual*> resultado;
        construirRedRecursivo(proyectos.getCabeza(), resultado);
        return resultado;
    }
};
