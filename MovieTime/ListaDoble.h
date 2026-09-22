#pragma once
#include "NodoDoble.h"
#include <iostream>
#include <functional>

template <class T>
class ListaDoble {
private:
    NodoDoble<T>* cabeza;
    NodoDoble<T>* cola;
    int longitud;

public:
    ListaDoble() : cabeza(nullptr), cola(nullptr), longitud(0) {}

    ~ListaDoble() {
        limpiar();
    }

    int getLongitud() const { return longitud; }
    bool esVacia() const { return cabeza == nullptr; }
    NodoDoble<T>* getCabeza() const { return cabeza; }
    NodoDoble<T>* getCola() const { return cola; }

    void agregarInicio(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        }
        else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
        longitud++;
    }

    void agregarFinal(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        longitud++;
    }

    void eliminarInicio() {
        if (cabeza == nullptr) return;
        NodoDoble<T>* aux = cabeza;
        if (cabeza == cola) {
            cabeza = cola = nullptr;
        }
        else {
            cabeza = cabeza->siguiente;
            cabeza->anterior = nullptr;
        }
        delete aux;
        longitud--;
    }

    void eliminarFinal() {
        if (cola == nullptr) return;
        NodoDoble<T>* aux = cola;
        if (cabeza == cola) {
            cabeza = cola = nullptr;
        }
        else {
            cola = cola->anterior;
            cola->siguiente = nullptr;
        }
        delete aux;
        longitud--;
    }

    void limpiar() {
        while (!esVacia()) {
            eliminarInicio();
        }
    }

    void recorrer(std::function<void(T)> accion) const {
        NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            accion(aux->valor);
            aux = aux->siguiente;
        }
    }

    void recorrerInverso(std::function<void(T)> accion) const {
        NodoDoble<T>* aux = cola;
        while (aux != nullptr) {
            accion(aux->valor);
            aux = aux->anterior;
        }
    }

    T* buscarSi(std::function<bool(T)> criterio) {
        NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) {
                return &(aux->valor);
            }
            aux = aux->siguiente;
        }
        return nullptr;
    }

    ListaDoble<T> filtrar(std::function<bool(T)> criterio) const {
        ListaDoble<T> resultado;
        NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) {
                resultado.agregarFinal(aux->valor);
            }
            aux = aux->siguiente;
        }
        return resultado;
    }
};