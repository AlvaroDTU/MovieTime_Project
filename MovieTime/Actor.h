#pragma once
#include <vector>
#include "Persona.h"

class Audiovisual;

class Actor : public Persona
{
private:
	std::vector<Audiovisual*> proyectos;
public:
	Actor(std::string nombres, std::string apellidos) : 
		Persona(nombres,apellidos) {}
	~Actor() {}
};

