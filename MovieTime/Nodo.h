#pragma once
template <typename T>
class Nodo
{
public:
	T valor;
	Nodo<T>* siguiente;
	Nodo(T v) : valor(v), siguiente(nullptr) {}
};

