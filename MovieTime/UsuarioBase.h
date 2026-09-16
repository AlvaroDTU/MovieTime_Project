#pragma once
#include <string>

using namespace std;

class UsuarioBase
{
protected:
	string id;
	string nombres;
	string apellidos;
	string nacionalidad;
	string contraseña;
	string correo;
	string fechaNacimiento;

public:
	UsuarioBase();
	UsuarioBase(string idd, string nom, string ape, string naci, string contr, string coreo, string fecha)
		: id(idd), nombres(nom), apellidos(ape), nacionalidad(naci), correo(coreo), contraseña(contr), fechaNacimiento(fecha)
	{}

	string getId() { return id; }
	string getNombres() { return nombres; }
	string getApellidos() { return apellidos; }
	string getNacionalidad() { return nacionalidad; }
	string getContraseña() { return contraseña; }
	string getCorreo() { return correo; }
	
	bool autenticar(string coreo, string contra) {
		if (coreo == correo && contra == contraseña) { return true; }
		else {return false;}}

	virtual void mostrarPerfil() = 0;
	virtual void mostrarMenu() = 0;

};

