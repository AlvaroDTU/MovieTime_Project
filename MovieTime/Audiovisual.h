#pragma once
#include <string>
#include <iostream>
#include "ListaSimple.h"
#include "Categoria.h"
#include "Director.h"
#include "Actor.h"
#include "Resenia.h"

class Audiovisual {
protected:
    int id;
    std::string titulo;
    int anio;
    Categoria* categoria;
    double rating;
    int popularidad;
    ListaSimple<Director*> directores;
    ListaSimple<Actor*> actores;
    ListaSimple<Resenia*> resenias;

public:
    Audiovisual(int id, std::string t, int a, Categoria* c)
        : id(id), titulo(t), anio(a), categoria(c), rating(0.0), popularidad(0) {
    }

    virtual ~Audiovisual() {
        directores.recorrer([](Director* d) { delete d; });
        actores.recorrer([](Actor* a) { delete a; });
        resenias.recorrer([](Resenia* r) { delete r; });
    }

    int getId() const { return id; }
    std::string getTitulo() const { return titulo; }
    int getAnio() const { return anio; }
    Categoria* getCategoria() const { return categoria; }
    double getRating() const { return rating; }
    int getPopularidad() const { return popularidad; }

    const ListaSimple<Director*>& getDirectores() const { return directores; }
    const ListaSimple<Actor*>& getActores() const { return actores; }

    void setRating(double r) { rating = r; }
    void setPopularidad(int p) { popularidad = p; }

    void agregarDirector(Director* d) { directores.agregarInicio(d); }
    void agregarActor(Actor* a) { actores.agregarInicio(a); }

    void agregarResenia(Resenia* r) {
        resenias.agregarInicio(r);
        popularidad++;
        recalcularRating();
    }

    void recalcularRating() {
        if (resenias.esVacia()) return;
        double suma = 0.0;
        int contador = 0;

        resenias.recorrer([&suma, &contador](Resenia* r) {
            suma += r->getEstrellas();
            contador++;
            });

        rating = suma / contador;
    }

    // Métodos virtuales puros que DEBEN ser sobrescritos
    virtual void mostrar() const = 0;
    virtual std::string getTipo() const = 0;
};