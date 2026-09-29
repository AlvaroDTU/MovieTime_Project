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
    double rating;       // Promedio sobre 10 estrellas
    int popularidad;     // Numero de interacciones / reseñas
    ListaSimple<Director*> directores;
    ListaSimple<Actor*> actores;
    ListaSimple<Resenia*> resenias;

public:
    Audiovisual(int id, std::string t, int a, Categoria* c)
        : id(id), titulo(t), anio(a), categoria(c), rating(0.0), popularidad(0) {}

    virtual ~Audiovisual() {
        resenias.recorrer([](Resenia* r) { delete r; });
        // Actores/Directores no se destruyen aqui: son compartidos entre varios
        // Audiovisual y su ciclo de vida lo administra el catalogo/GestorArchivos.
    }

    int getId() const { return id; }
    std::string getTitulo() const { return titulo; }
    int getAnio() const { return anio; }
    Categoria* getCategoria() const { return categoria; }
    double getRating() const { return rating; }         // sobre 10
    int getPopularidad() const { return popularidad; }

    const ListaSimple<Director*>& getDirectores() const { return directores; }
    const ListaSimple<Actor*>& getActores() const { return actores; }
    const ListaSimple<Resenia*>& getResenias() const { return resenias; }

    void setRating(double r) { rating = r; }
    void setPopularidad(int p) { popularidad = p; }

    void agregarDirector(Director* d) { directores.agregarInicio(d); }
    void agregarActor(Actor* a) { actores.agregarInicio(a); }

    void agregarResenia(Resenia* r) {
        resenias.agregarInicio(r);
        popularidad++;
        recalcularRating();
    }

    // Recalcula el promedio usando una lambda "extractor" sobre la calificacion (0-10)
    void recalcularRating() {
        if (resenias.esVacia()) { rating = 0.0; return; }
        double suma = resenias.acumularRecursivo([](Resenia* r) { return r->getCalificacion(); });
        rating = suma / resenias.getLongitud();
    }

    // Metodo virtual polimorfico: minutos totales reproducidos/estimados,
    // usado por la metrica recursiva de "Conteo/Recorrido Recursivo".
    virtual int getMinutosTotales() const = 0;

    virtual void mostrar() const = 0;
    virtual std::string getTipo() const = 0;
};
