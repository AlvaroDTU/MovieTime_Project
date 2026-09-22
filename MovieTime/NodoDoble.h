#pragma once

template <class T>
class NodoDoble {
public:
    T valor;
    NodoDoble<T>* siguiente;
    NodoDoble<T>* anterior;

    NodoDoble(T v) : valor(v), siguiente(nullptr), anterior(nullptr) {}
};