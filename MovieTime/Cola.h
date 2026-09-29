#pragma once
#include "Nodo.h"
#include <iostream>
#include <functional>

template <class T>
class Cola {
private:
    Nodo<T>* inicio;
    Nodo<T>* fin;
    int longitud;

public:
    Cola() : inicio(nullptr), fin(nullptr), longitud(0) {}

    ~Cola() {
        while (!empty()) extraer();
    }

    void insertar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (empty()) { inicio = fin = nuevo; }
        else { fin->siguiente = nuevo; fin = nuevo; }
        longitud++;
    }

    T extraer() {
        if (empty()) {
            std::cout << "La cola esta vacia." << std::endl;
            return T();
        }
        Nodo<T>* aux = inicio;
        T valor = aux->valor;
        inicio = inicio->siguiente;
        if (inicio == nullptr) fin = nullptr;
        delete aux;
        longitud--;
        return valor;
    }

    bool empty() const { return longitud == 0; }
    int getLongitud() const { return longitud; }

    // Permite mostrar/inspeccionar las tendencias sin desencolarlas.
    void recorrer(std::function<void(T)> accion) const {
        Nodo<T>* aux = inicio;
        while (aux != nullptr) { accion(aux->valor); aux = aux->siguiente; }
    }

    // Evita duplicar el mismo elemento dos veces en tendencias.
    bool contiene(std::function<bool(T)> criterio) const {
        Nodo<T>* aux = inicio;
        while (aux != nullptr) {
            if (criterio(aux->valor)) return true;
            aux = aux->siguiente;
        }
        return false;
    }
};