#pragma once
#include <string>
#include <iostream>
#include "Categoria.h"
#include "Resenia.h"
#include "ListaSimple.h"

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

    // Cambiados de std::vector a ListaSimple
    ListaSimple<Resenia*> resenias;
    ListaSimple<Actor*> actores;
    ListaSimple<Director*> directores;

public:
    Audiovisual(int id, const std::string& titulo, int anio, Categoria* categoria)
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

    // Gestión de Listas Simples
    void agregarActor(Actor* actor) { actores.agregarInicio(actor); }
    void agregarDirector(Director* director) { directores.agregarInicio(director); }
    void agregarResenia(Resenia* resenia) { resenias.agregarInicio(resenia); }

    ListaSimple<Actor*>& getActores() { return actores; }
    ListaSimple<Director*>& getDirectores() { return directores; }
    ListaSimple<Resenia*>& getResenias() { return resenias; }

    bool operator==(const Audiovisual& otra) const { return id == otra.id; }
    bool operator>(const Audiovisual& otra) const { return rating > otra.rating; }

    virtual std::string getTipo() const = 0;
    virtual void mostrar() const = 0;
};