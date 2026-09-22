#pragma once
#include <iostream>

class Categoria
{
	int i = 0;
private:
	int id;
	std::string nombre;
public:
	Categoria(int id, std::string nombre):
		id(id), nombre(nombre) {}

	int getId() { return id; }
	std::string getNombre() { return nombre; }

	void mostrar() 
	{
		std::cout << "Categoria [" << id << "] " << nombre << "\n";
	}
};

