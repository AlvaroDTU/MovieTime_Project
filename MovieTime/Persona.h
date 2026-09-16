#pragma once
#include <string>

class Persona
{
protected:
	std::string nombres;
	std::string apellidos;
public:
	Persona(std::string nombres, std::string apellidos) : 
		nombres(nombres), apellidos(apellidos) {}
	~Persona() {}
	std::string getNombres() { return nombres; }
	std::string getApellidos() { return apellidos; }
};

