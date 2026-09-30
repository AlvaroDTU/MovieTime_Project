#pragma once
// ============================================================================
//  RegistroUsuario.h
//  Modulo independiente para registrar nuevos usuarios (rol CLIENTE) en
//  Movie Time. No modifica ninguna clase existente: reutiliza UsuarioCliente,
//  ListaSimple y GestorArchivos::cargarUsuarios / guardarUsuarios.
//
//  Como es header-only (igual que el resto del proyecto) no necesita pch.h.
// ============================================================================
#include <iostream>
#include <string>
#include <functional>
#include <cctype>
#include <ctime>
#include "ListaSimple.h"
#include "UsuarioBase.h"
#include "UsuarioCliente.h"
#include "GestorArchivos.h"

class RegistroUsuario {
private:
    // ---------------- Utilidades de texto ----------------
    static std::string recortar(const std::string& s) {
        size_t ini = 0, fin = s.size();
        while (ini < fin && std::isspace(static_cast<unsigned char>(s[ini]))) ini++;
        while (fin > ini && std::isspace(static_cast<unsigned char>(s[fin - 1]))) fin--;
        return s.substr(ini, fin - ini);
    }
    static std::string aMinusculas(std::string s) {
        for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }
    static bool contieneComa(const std::string& s) {
        return s.find(',') != std::string::npos;
    }
    static bool contieneEspacios(const std::string& s) {
        for (char c : s) if (std::isspace(static_cast<unsigned char>(c))) return true;
        return false;
    }
    static bool contieneDigitos(const std::string& s) {
        for (char c : s) if (std::isdigit(static_cast<unsigned char>(c))) return true;
        return false;
    }
    // ---------------- Validaciones (devuelven "" si es valido) ----------------
    // La coma esta prohibida porque usuarios.txt es un CSV simple.
    static std::string errorTextoPersona(const std::string& s, const std::string& campo) {
        if (s.empty()) return campo + " no puede estar vacio.";
        if (s.size() < 2) return campo + " debe tener al menos 2 caracteres.";
        if (contieneComa(s)) return campo + " no puede contener comas.";
        if (contieneDigitos(s)) return campo + " no puede contener numeros.";
        return "";
    }
    // El login usa "cin >> correo", por eso no se admiten espacios.
    static std::string errorCorreo(const std::string& s, ListaSimple<UsuarioBase*>& usuarios) {
        if (s.empty()) return "El correo no puede estar vacio.";
        if (contieneEspacios(s) || contieneComa(s)) return "El correo no puede tener espacios ni comas.";

        size_t arroba = s.find('@');
        if (arroba == std::string::npos || arroba == 0 || s.find('@', arroba + 1) != std::string::npos)
            return "Formato invalido (ejemplo: nombre@dominio.com).";

        std::string dominio = s.substr(arroba + 1);
        size_t punto = dominio.find('.');
        if (punto == std::string::npos || punto == 0 || dominio.back() == '.')
            return "Formato invalido (ejemplo: nombre@dominio.com).";

        std::string buscado = aMinusculas(s);
        UsuarioBase** existente = usuarios.buscarSi([&buscado](UsuarioBase* u) {
            return aMinusculas(u->getCorreo()) == buscado;
            });
        if (existente != nullptr) return "Ya existe un usuario registrado con ese correo.";
        return "";
    }
    // El login usa "cin >> contra", por eso no se admiten espacios.
    static std::string errorContrasenia(const std::string& s) {
        if (s.size() < 6) return "La contrasena debe tener al menos 6 caracteres.";
        if (contieneEspacios(s) || contieneComa(s)) return "La contrasena no puede tener espacios ni comas.";
        return "";
    }
    // Formato AAAA-MM-DD, fecha real y no futura.
    static std::string errorFecha(const std::string& f) {
        const std::string formato = "Use el formato AAAA-MM-DD (ejemplo: 2000-01-31).";
        if (f.size() != 10 || f[4] != '-' || f[7] != '-') return formato;
        for (size_t i = 0; i < f.size(); i++) {
            if (i == 4 || i == 7) continue;
            if (!std::isdigit(static_cast<unsigned char>(f[i]))) return formato;
        }
        int anio = std::stoi(f.substr(0, 4));
        int mes = std::stoi(f.substr(5, 2));
        int dia = std::stoi(f.substr(8, 2));

        if (anio < 1900) return "El anio debe ser 1900 o posterior.";
        if (mes < 1 || mes > 12) return "El mes debe estar entre 01 y 12.";

        int diasMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto) diasMes[1] = 29;
        if (dia < 1 || dia > diasMes[mes - 1]) return "Ese dia no existe en el mes indicado.";

        std::time_t ahora = std::time(nullptr);
        std::tm local;
#ifdef _MSC_VER
        localtime_s(&local, &ahora);
#else
        localtime_r(&ahora, &local);
#endif
        int hoy = (local.tm_year + 1900) * 10000 + (local.tm_mon + 1) * 100 + local.tm_mday;
        if (anio * 10000 + mes * 100 + dia > hoy) return "La fecha de nacimiento no puede ser futura.";
        return "";
    }
    // ---------------- Entrada de datos ----------------
    // Devuelve false si el usuario escribe 0 (cancelar) o se acaba la entrada.
    static bool leer(const std::string& mensaje, std::string& destino) {
        std::cout << mensaje;
        if (!std::getline(std::cin, destino)) return false;
        destino = recortar(destino);
        return destino != "0";
    }

    // Repite la pregunta hasta que el dato sea valido o el usuario cancele.
    static bool pedir(const std::string& mensaje, std::string& destino,
        std::function<std::string(const std::string&)> validar) {
        while (true) {
            if (!leer(mensaje, destino)) return false;
            std::string error = validar(destino);
            if (error.empty()) return true;
            std::cout << "[!] " << error << "\n";
        }
    }
    // ID autoincremental: mayor ID numerico existente + 1.
    static std::string generarId(ListaSimple<UsuarioBase*>& usuarios) {
        int maximo = 0;
        usuarios.recorrer([&maximo](UsuarioBase* u) {
            const std::string id = u->getId();
            if (id.empty() || id.size() > 9) return;
            for (char c : id) if (!std::isdigit(static_cast<unsigned char>(c))) return;
            int n = std::stoi(id);
            if (n > maximo) maximo = n;
            });
        return std::to_string(maximo + 1);
    }
public:
    // Registra un cliente en la lista recibida y guarda todo en "archivo".
    // Devuelve true si el registro se completo.
    static bool registrar(ListaSimple<UsuarioBase*>& usuarios, const std::string& archivo = "usuarios.txt") {
        std::string nombres, apellidos, nacionalidad, correo, contrasenia, confirmacion, fecha;
        std::cout << "\n========================================\n";
        std::cout << "         REGISTRAR NUEVO USUARIO        \n";
        std::cout << "========================================\n";
        std::cout << "(Escriba 0 en cualquier campo para cancelar)\n\n";
        bool ok =
            pedir("Nombres: ", nombres,
                [](const std::string& s) { return errorTextoPersona(s, "El nombre"); }) &&
            pedir("Apellidos: ", apellidos,
                [](const std::string& s) { return errorTextoPersona(s, "El apellido"); }) &&
            pedir("Nacionalidad: ", nacionalidad,
                [](const std::string& s) { return errorTextoPersona(s, "La nacionalidad"); }) &&
            pedir("Fecha de nacimiento (AAAA-MM-DD): ", fecha,
                [](const std::string& s) { return errorFecha(s); }) &&
            pedir("Correo: ", correo,
                [&usuarios](const std::string& s) { return errorCorreo(s, usuarios); });
        if (!ok) { std::cout << "\n[!] Registro cancelado.\n"; return false; }
        // Contrasena con confirmacion
        while (true) {
            if (!pedir("Contrasena (min. 6 caracteres): ", contrasenia,
                [](const std::string& s) { return errorContrasenia(s); })) {
                std::cout << "\n[!] Registro cancelado.\n";
                return false;
            }
            if (!leer("Confirmar contrasena: ", confirmacion)) {
                std::cout << "\n[!] Registro cancelado.\n";
                return false;
            }
            if (confirmacion == contrasenia) break;
            std::cout << "[!] Las contrasenas no coinciden. Intente de nuevo.\n";
        }
        // Resumen y confirmacion final
        std::cout << "\n--- RESUMEN ---\n"
            << "Nombre: " << nombres << " " << apellidos << "\n"
            << "Nacionalidad: " << nacionalidad << "\n"
            << "Fecha de nacimiento: " << fecha << "\n"
            << "Correo: " << correo << "\n";
        std::string resp;
        if (!leer("\nConfirmar registro? (s/n): ", resp) || (resp != "s" && resp != "S")) {
            std::cout << "\n[!] Registro cancelado.\n";
            return false;
        }
        std::string id = generarId(usuarios);
        usuarios.agregarFinal(new UsuarioCliente(id, nombres, apellidos, nacionalidad, contrasenia, correo, fecha));
        GestorArchivos::guardarUsuarios(archivo, usuarios);
        std::cout << "\n[OK] Usuario registrado con ID " << id << ". Ya puede iniciar sesion con su correo.\n";
        return true;
    }
    // Version autonoma: carga usuarios.txt, registra y lo vuelve a guardar.
    // Usar ANTES de Sistema::iniciar() (el Sistema carga usuarios.txt al arrancar).
    static bool registrarEnArchivo(const std::string& archivo = "usuarios.txt") {
        ListaSimple<UsuarioBase*> usuarios;
        GestorArchivos::cargarUsuarios(archivo, usuarios);
        bool ok = registrar(usuarios, archivo);
        usuarios.recorrer([](UsuarioBase* u) { delete u; });
        return ok;
    }
    // Menu previo al login: registrar usuarios o continuar al sistema.
    static void menuPrevio(const std::string& archivo = "usuarios.txt") {
        std::string op;
        while (true) {
            std::cout << "\n========================================\n";
            std::cout << "       MOVIE TIME - BIENVENIDO(A)       \n";
            std::cout << "========================================\n";
            std::cout << "1. Registrar nuevo usuario\n";
            std::cout << "2. Continuar al inicio de sesion\n";
            std::cout << "Opcion: ";
            if (!std::getline(std::cin, op)) return;
            op = recortar(op);
            if (op == "1") registrarEnArchivo(archivo);
            else if (op == "2") return;
            else std::cout << "[!] Opcion no valida.\n";
        }
    }
};