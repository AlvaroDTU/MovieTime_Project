#pragma once
#include "Persona.h"
class Director : public Persona
{
private:
	//vector de peliculas que hizo
public:
	Director(std::string nombres, std::string apellidos) :
		Persona(nombres, apellidos) {}
	~Director() {}
};

