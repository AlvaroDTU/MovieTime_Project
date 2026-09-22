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

    void recorrer() {
        if (cabeza == nullptr) {
            cout << "\nLista Vacia" << endl;
            return;
        }

        Nodo<T>* aux = cabeza;
        while (aux != nullptr) {
            // Nota: Si T es un objeto como Empleado, se asume que 'valor' tiene un método mostrar() o toString()
            // Si T es un entero/float, usas: cout << aux->valor << " ";
            aux->valor.mostrar();
            aux = aux->siguiente;
        }
        cout << endl;
    }
};