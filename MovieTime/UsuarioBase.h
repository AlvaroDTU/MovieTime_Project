#pragma once
#include <string>
#include <iostream>


class UsuarioBase
{
protected:
	std::string id;
	std::string nombres;
	std::string apellidos;
	std::string nacionalidad;
	std::string contraseña;
	std::string correo;
	std::string fechaNacimiento;

public:
	UsuarioBase();
	UsuarioBase(std::string idd, std::string nom, std::string ape, std::string naci, std::string contr, std::string coreo, std::string fecha)
		: id(idd), nombres(nom), apellidos(ape), nacionalidad(naci), correo(coreo), contraseña(contr), fechaNacimiento(fecha)
	{}

	std::string getId() { return id; }
	std::string getNombres() { return nombres; }
	std::string getApellidos() { return apellidos; }
	std::string getNacionalidad() { return nacionalidad; }
	std::string getContraseña() { return contraseña; }
	std::string getCorreo() { return correo; }
	
	bool autenticar(std::string coreo, std::string contra) {
		if (coreo == correo && contra == contraseña) { return true; }
		else {return false;}}

	virtual void mostrarPerfil() = 0;
	virtual void mostrarMenu() = 0;

};

