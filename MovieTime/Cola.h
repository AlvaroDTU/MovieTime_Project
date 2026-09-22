#pragma once
#include "Nodo.h"
#include <iostream>
using namespace std;

template <class T>
class Cola {
private:
	Nodo<T>* inicio;
	Nodo<T>* fin;
	int longitud;
public:
	Cola() : inicio(nullptr), fin(nullptr), longitud(0) {}
	void insertar(T valor)
	{
		Nodo<T>* nuevo = new Nodo<T>(valor);
		if (empty())
		{
			inicio = nuevo;
			fin = inicio;
		}
		else
		{
			fin->siguiente = nuevo;
			fin = nuevo;
		}
	}
	T extraer()
	{
		T dato = inicio->dato;
		if (inicio == fin)
		{
			inicio = nullptr;
			fin = nullptr;
		}
		else
		{
			inicio = inicio->siguiente;
		}

		return dato;
	}
	bool empty() { return longitud == 0; }
};