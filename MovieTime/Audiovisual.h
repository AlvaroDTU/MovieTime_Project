#pragma once
#include <string>
#include <iostream>
#include "ListaSimple.h"
#include "Categoria.h"
#include "Resenia.h"

class Audiovisual {
protected:
    int id;
    std::string titulo;
    int anio;
    Categoria* categoria;
    double rating;
    int popularidad;
    ListaSimple<Resenia*> resenias;

public:
    Audiovisual(int id, std::string t, int a, Categoria* c)
        : id(id), titulo(t), anio(a), categoria(c), rating(0.0), popularidad(0) {
    }

    virtual ~Audiovisual() {
        resenias.recorrer([](Resenia* r) { delete r; });
    }

    int getId() const { return id; }
    std::string getTitulo() const { return titulo; }
    int getAnio() const { return anio; }
    Categoria* getCategoria() const { return categoria; }
    double getRating() const { return rating; }
    int getPopularidad() const { return popularidad; }

    void setRating(double r) { rating = r; }
    void setPopularidad(int p) { popularidad = p; }

    void agregarResenia(Resenia* r) {
        resenias.agregarInicio(r);
        popularidad++; // Aumenta la interacción
        recalcularRating();
    }

    void recalcularRating() {
        if (resenias.esVacia()) return;
        double suma = 0.0;
        int contador = 0;

        resenias.recorrer([&suma, &contador](Resenia* r) {
            suma += r->getEstrellas(); // Asegúrate de tener getEstrellas() en Resenia.h
            contador++;
            });

        rating = suma / contador;
    }

    virtual void mostrar() = 0;
    virtual std::string getTipo() = 0;
};