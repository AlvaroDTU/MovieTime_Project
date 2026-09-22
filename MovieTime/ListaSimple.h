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
    Nodo<T>* getCabeza() const { return cabeza; }

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
            while (aux->siguiente != nullptr) {
                aux = aux->siguiente;
            }
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
        while (aux->siguiente->siguiente != nullptr) {
            aux = aux->siguiente;
        }

        Nodo<T>* aux2 = aux->siguiente;
        aux->siguiente = nullptr;
        delete aux2;
        longitud--;
    }

    void recorrer(std::function<void(T)> accion) const {
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            accion(aux->valor);
            aux = aux->siguiente;
        }
    }

    T* buscarSi(std::function<bool(T)> criterio) {
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) {
                return &(aux->valor);
            }
            aux = aux->siguiente;
        }
        return nullptr;
    }

    ListaSimple<T> filtrar(std::function<bool(T)> criterio) const {
        ListaSimple<T> resultado;
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) {
                resultado.agregarFinal(aux->valor);
            }
            aux = aux->siguiente;
        }
        return resultado;
    }

    // Método recursivo exigido en la rúbrica
    bool buscarRecursivo(Nodo<T>* nodoActual, std::function<bool(T)> criterio) {
        if (nodoActual == nullptr) return false;
        if (criterio(nodoActual->valor)) return true;
        return buscarRecursivo(nodoActual->siguiente, criterio);
    }
};

//// 1. Uso de recorrer: Imprime todas las películas
//miLista.recorrer([](Pelicula p) {
//    cout << "Película: " << p.getTitulo() << " - Rating: " << p.getRating() << endl;
//    });
//
//// 2. Uso de buscarSi: Devuelve un puntero a "Inception"
//Pelicula* buscada = miLista.buscarSi([](Pelicula p) {
//    return p.getTitulo() == "Inception";
//    });
//
//if (buscada != nullptr) {
//    cout << "Encontrada! Director: " << buscada->getDirector() << endl;
//}
//
//// 3. Uso de filtrar: Obtiene una nueva ListaSimple solo con películas > 8.0 de rating
//ListaSimple<Pelicula> topPeliculas = miLista.filtrar([](Pelicula p) {
//    return p.getRating() > 8.0;
//    });
//
//// 4. Uso de buscarRecursivo: Retorna true si hay alguna película de 'Nolan'
//bool tieneNolan = miLista.buscarRecursivo(miLista.getCabeza(), [](Pelicula p) {
//    return p.getDirector() == "Christopher Nolan";
//    });