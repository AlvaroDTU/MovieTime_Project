#pragma once
#include "Nodo.h"
#include <iostream>
#include <functional>

template <class T>
class ListaSimple {
private:
    Nodo<T>* cabeza;
    int longitud;

public:
    ListaSimple() : cabeza(nullptr), longitud(0) {}

    ~ListaSimple() {
        while (cabeza != nullptr) {
            eliminarInicio();
        }
    }

    int getLongitud() const { return longitud; }
    bool esVacia() const { return cabeza == nullptr; }

    Nodo<T>* getCabeza() { return cabeza; }
    const Nodo<T>* getCabeza() const { return cabeza; }

    void agregarInicio(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        longitud++;
    }

    void agregarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        }
        else {
            Nodo<T>* aux = cabeza;
            while (aux->siguiente != nullptr) aux = aux->siguiente;
            aux->siguiente = nuevo;
        }
        longitud++;
    }

    void eliminarInicio() {
        if (cabeza == nullptr) return;
        Nodo<T>* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
        longitud--;
    }

    void eliminarFinal() {
        if (cabeza == nullptr) return;
        if (cabeza->siguiente == nullptr) {
            delete cabeza;
            cabeza = nullptr;
            longitud--;
            return;
        }
        Nodo<T>* aux = cabeza;
        while (aux->siguiente->siguiente != nullptr) aux = aux->siguiente;
        Nodo<T>* aux2 = aux->siguiente;
        aux->siguiente = nullptr;
        delete aux2;
        longitud--;
    }

    // --- Lambda: recorrido ---
    void recorrer(std::function<void(T)> accion) const {
        const Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            accion(aux->valor);
            aux = aux->siguiente;
        }
    }

    // --- Lambda: busqueda puntual ---
    T* buscarSi(std::function<bool(T)> criterio) {
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) return &(aux->valor);
            aux = aux->siguiente;
        }
        return nullptr;
    }

    const T* buscarSi(std::function<bool(T)> criterio) const {
        const Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) return &(aux->valor);
            aux = aux->siguiente;
        }
        return nullptr;
    }

    // --- Lambda: filtrado ---
    ListaSimple<T> filtrar(std::function<bool(T)> criterio) const {
        ListaSimple<T> resultado;
        const Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) resultado.agregarFinal(aux->valor);
            aux = aux->siguiente;
        }
        return resultado;
    }

    void actualizarSi(std::function<bool(T)> criterio, std::function<void(T&)> accionActualizar) {
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) accionActualizar(aux->valor);
            aux = aux->siguiente;
        }
    }

    bool buscarRecursivo(Nodo<T>* nodoActual, std::function<bool(T)> criterio) {
        if (nodoActual == nullptr) return false;
        if (criterio(nodoActual->valor)) return true;
        return buscarRecursivo(nodoActual->siguiente, criterio);
    }

    bool buscarRecursivo(std::function<bool(T)> criterio) {
        return buscarRecursivo(cabeza, criterio);
    }

    double acumularRecursivo(const Nodo<T>* nodoActual, std::function<double(T)> extractor) const {
        if (nodoActual == nullptr) return 0.0;
        return extractor(nodoActual->valor) + acumularRecursivo(nodoActual->siguiente, extractor);
    }

    double acumularRecursivo(std::function<double(T)> extractor) const {
        return acumularRecursivo(cabeza, extractor);
    }
};