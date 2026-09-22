#pragma once
#include <vector>
#include "Persona.h"
#include "Audiovisual.h"

class Audiovisual;

class Director : public Persona
{
private:
	std::vector<Audiovisual*> proyectosDirigidos;
public:
	Director(std::string nombres, std::string apellidos) :
		Persona(nombres, apellidos) 
	{
	}
	~Director() {}
};

