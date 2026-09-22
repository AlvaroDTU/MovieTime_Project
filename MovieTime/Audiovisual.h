#pragma once
#include <string>
#include <vector>
#include "Categoria.h"
#include "Resenia.h"

class Audiovisual
{
protected:
	int id;
	std::string titulo;
	int anio;
	double rating;
	int popularidad;
	Categoria* categoria;
	std::vector<Resenia*> resenias;
public:
	Audiovisual(int id, std::string titulo, int anio, Categoria* categoria):
		id(id), titulo(titulo), anio(anio), rating(0.0), popularidad(0), categoria(categoria) {}
	int getId() { return id; }
	std::string getTitulo() { return titulo; }
	int getAnio() { return anio; }
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

