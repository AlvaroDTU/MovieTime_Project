#pragma once
#include "Nodo.h"
#include <iostream>

template <class T>
class Cola {
private:
    Nodo<T>* inicio;
    Nodo<T>* fin;
    int longitud;

public:
    Cola() : inicio(nullptr), fin(nullptr), longitud(0) {}

    ~Cola() {
        while (!empty()) {
            extraer();
        }
    }

    void insertar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (empty()) {
            inicio = fin = nuevo;
        }
        else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
        longitud++; // <-- IMPORTANTE: Incrementar la longitud
    }

    T extraer() {
        if (empty()) {
            std::cout << "La cola esta vacia." << std::endl;
            return T();
        }

        Nodo<T>* aux = inicio;
        T valor = aux->valor; // Corregido: 'valor' en lugar de 'dato'

        inicio = inicio->siguiente;
        if (inicio == nullptr) {
            fin = nullptr;
        }

        delete aux; // Liberar memoria del nodo extraído
        longitud--; // <-- IMPORTANTE: Decrementar la longitud
        return valor;
    }

    bool empty() const { return longitud == 0; }
    int getLongitud() const { return longitud; }
};