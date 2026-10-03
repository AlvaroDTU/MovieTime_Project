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

    ~ListaDoble() { limpiar(); }

    int getLongitud() const { return longitud; }
    bool esVacia() const { return cabeza == nullptr; }

    NodoDoble<T>* getCabeza() { return cabeza; }
    const NodoDoble<T>* getCabeza() const { return cabeza; }

    NodoDoble<T>* getCola() { return cola; }
    const NodoDoble<T>* getCola() const { return cola; }

    void agregarInicio(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) { cabeza = cola = nuevo; }
        else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
        longitud++;
    }

    void agregarFinal(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) { cabeza = cola = nuevo; }
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
        if (cabeza == cola) { cabeza = cola = nullptr; }
        else { cabeza = cabeza->siguiente; cabeza->anterior = nullptr; }
        delete aux;
        longitud--;
    }

    void eliminarFinal() {
        if (cola == nullptr) return;
        NodoDoble<T>* aux = cola;
        if (cabeza == cola) { cabeza = cola = nullptr; }
        else { cola = cola->anterior; cola->siguiente = nullptr; }
        delete aux;
        longitud--;
    }

    void limpiar() {
        while (!esVacia()) eliminarInicio();
    }

    // --- Lambda: recorridos en ambos sentidos ---
    void recorrer(std::function<void(T)> accion) const {
        const NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) { accion(aux->valor); aux = aux->siguiente; }
    }

    void recorrerInverso(std::function<void(T)> accion) const {
        const NodoDoble<T>* aux = cola;
        while (aux != nullptr) { accion(aux->valor); aux = aux->anterior; }
    }

    // --- Lambda: busqueda puntual ---
    T* buscarSi(std::function<bool(T)> criterio) {
        NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) return &(aux->valor);
            aux = aux->siguiente;
        }
        return nullptr;
    }

    const T* buscarSi(std::function<bool(T)> criterio) const {
        const NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) return &(aux->valor);
            aux = aux->siguiente;
        }
        return nullptr;
    }

    // --- Lambda: filtrado ---
    ListaDoble<T> filtrar(std::function<bool(T)> criterio) const {
        ListaDoble<T> resultado;
        const NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) resultado.agregarFinal(aux->valor);
            aux = aux->siguiente;
        }
        return resultado;
    }

    // --- Lambda ---
    void actualizarSi(std::function<bool(T)> criterio, std::function<void(T&)> accionActualizar) {
        NodoDoble<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) accionActualizar(aux->valor);
            aux = aux->siguiente;
        }
    }

    T* buscarRecursivo(NodoDoble<T>* nodoActual, std::function<bool(T)> criterio) {
        if (nodoActual == nullptr) return nullptr;
        if (criterio(nodoActual->valor)) return &(nodoActual->valor);
        return buscarRecursivo(nodoActual->siguiente, criterio);
    }

    T* buscarRecursivo(std::function<bool(T)> criterio) {
        return buscarRecursivo(cabeza, criterio);
    }

    T* aArreglo() const {
        if (esVacia()) return nullptr;
        T* arreglo = new T[longitud];
        const NodoDoble<T>* aux = cabeza;
        int i = 0;
        while (aux != nullptr) { arreglo[i++] = aux->valor; aux = aux->siguiente; }
        return arreglo;
    }

    void reordenarDesde(T* arregloOrdenado) {
        NodoDoble<T>* aux = cabeza;
        int i = 0;
        while (aux != nullptr) { aux->valor = arregloOrdenado[i++]; aux = aux->siguiente; }
    }
};