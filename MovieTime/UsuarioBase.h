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
    std::string contrasenia;
    std::string correo;
    std::string fechaNacimiento;

public:
    UsuarioBase(std::string idd, std::string nom, std::string ape, std::string naci,
        std::string contr, std::string coreo, std::string fecha)
        : id(idd), nombres(nom), apellidos(ape), nacionalidad(naci),
        contrasenia(contr), correo(coreo), fechaNacimiento(fecha)
    {}

    virtual ~UsuarioBase() {}

    std::string getId() const { return id; }
    std::string getNombres() const { return nombres; }
    std::string getApellidos() const { return apellidos; }
    std::string getNacionalidad() const { return nacionalidad; }
    std::string getContrasenia() const { return contrasenia; }
    std::string getCorreo() const { return correo; }
    std::string getFechaNacimiento() const { return fechaNacimiento; }

    bool autenticar(const std::string& coreo, const std::string& contra) const {
        return (coreo == correo && contra == contrasenia);
    }

    virtual void mostrarPerfil() = 0;
    virtual void mostrarMenu() = 0;
    virtual std::string getRol() const = 0; 
};
