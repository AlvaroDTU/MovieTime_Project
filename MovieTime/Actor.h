#pragma once
#include "Persona.h"
class Actor : public Persona
{
private:
	// vector de peliculas en las que aparece
public:
	Actor(std::string nombres, std::string apellidos) : 
		Persona(nombres,apellidos) {}
	~Actor() {}
};

