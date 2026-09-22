#pragma once
#include "Nodo.h"
#include <iostream>
using namespace std;

template <class T>
class ListaSimple {
private:
    Nodo<T>* cabeza;
    int longitud;

public:
    ListaSimple() : cabeza(nullptr), longitud(0) {}

    void agregarFinal(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);

        if (cabeza == nullptr) {
            cabeza = nuevo;
            longitud++;
            return;
        }

        Nodo<T>* aux = cabeza;
        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }

        aux->siguiente = nuevo;
        longitud++;
    }

    void agregarPos(T valor, int pos) {
        if (pos == 0) {
            agregarInicio(valor);
        }
        else if (pos == longitud) {
            agregarFinal(valor);
        }
        else if (pos > longitud || pos < 0) {
            cout << "Posicion invalida, longitud de la lista es de: " << longitud << " elementos" << endl;
        }
        else {
            Nodo<T>* nuevo = new Nodo<T>(valor);
            Nodo<T>* aux = cabeza;

            // Nos detenemos en el nodo anterior a la posición deseada
            for (int i = 0; i < pos - 1; i++) {
                aux = aux->siguiente;
            }

            nuevo->siguiente = aux->siguiente;
            aux->siguiente = nuevo;
            longitud++;
        }
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

        Nodo<T>* aux2 = aux->siguiente; // Nodo a eliminar
        aux->siguiente = nullptr;
        delete aux2;
        longitud--;
    }

    void eliminarPos(int pos) {
        if (pos == 0) {
            eliminarInicio();
        }
        else if (pos == longitud - 1) {
            eliminarFinal();
        }
        else if (pos >= longitud || pos < 0) {
            cout << "Posicion invalida, longitud de la lista es de: " << longitud << " elementos" << endl;
        }
        else {
            Nodo<T>* aux = cabeza;

            for (int i = 0; i < pos - 1; i++) {
                aux = aux->siguiente;
            }

            Nodo<T>* aux2 = aux->siguiente; // Nodo a eliminar
            aux->siguiente = aux2->siguiente;
            delete aux2;
            longitud--;
        }
    }

    void recorrer(function<void(T)> accion) const {
        if (cabeza == nullptr) {
            cout << "\nLista Vacia" << endl;
            return;
        }
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            accion(aux->valor); // Pasa cada elemento a la función lambda
            aux = aux->siguiente;
        }
    }

    // 2. Buscar por Condición / Criterio (Retorna puntero o elemento encontrado)
    T* buscarSi(function<bool(T)> criterio) {
        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            if (criterio(aux->valor)) {
                return &(aux->valor);
            }
            aux = aux->siguiente;
        }
        return nullptr;
    }

    // 3. Filtrar Elementos (Retorna una nueva ListaSimple con los elementos filtrados)
    ListaSimple<T> filtrar(function<bool(T)> criterio) const {
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

    // --- MÉTODO RECURSIVO EXIGIDO EN LA RÚBRICA ---
    // Buscar la presencia de un elemento de forma recursiva
    bool buscarRecursivo(Nodo<T>* nodoActual, function<bool(T)> criterio) {
        if (nodoActual == nullptr) return false;                 // Caso Base 1: No encontrado
        if (criterio(nodoActual->valor)) return true;             // Caso Base 2: Encontrado
        return buscarRecursivo(nodoActual->siguiente, criterio); // Paso Recursivo
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