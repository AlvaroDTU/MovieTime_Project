#pragma once
#include <string>

class Resenia
{
private:
	int estrellas;
	std::string comentario;
public:
	Resenia(int estrellas, std::string comentario):
		estrellas(estrellas), comentario(comentario) {}
};
