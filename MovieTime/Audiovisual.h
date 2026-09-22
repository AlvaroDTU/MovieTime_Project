#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Categoria.h"
#include "Resenia.h"
// #include "ListaSimple.h" // <-- Para reemplazar el vector más adelante

class Actor;
class Director;

class Audiovisual {
protected:
	int id;
	std::string titulo;
	int anio;
	double rating;
	int popularidad;
	Categoria* categoria;
	std::vector<Resenia*> resenias;
	std::vector<Actor*> actores;
	std::vector<Director*> directores;

    // cambiar a listaSimple
public:
    Audiovisual(int id, std::string titulo, int anio, Categoria* categoria)
        : id(id), titulo(titulo), anio(anio), rating(0.0), popularidad(0), categoria(categoria) {
    }

    virtual ~Audiovisual() {}

    int getId() const { return id; }
    std::string getTitulo() const { return titulo; }
    int getAnio() const { return anio; }
    double getRating() const { return rating; }
    int getPopularidad() const { return popularidad; }
    Categoria* getCategoria() const { return categoria; }

    void setId(int i) { id = i; }
    void setTitulo(const std::string& t) { titulo = t; }
    void setAnio(int a) { anio = a; }
    void setRating(double r) { rating = r; }
    void setPopularidad(int p) { popularidad = p; }
    void setCategoria(Categoria* c) { categoria = c; }

    void incrementarPopularidad() { ++popularidad; }

    bool operator==(const Audiovisual& otra) const { return id == otra.id; }
    bool operator>(const Audiovisual& otra) const { return rating > otra.rating; }

    virtual std::string getTipo() const = 0;
    virtual void mostrar() const = 0;
};