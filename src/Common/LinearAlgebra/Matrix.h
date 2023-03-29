// Matrix.h
// classe per il contenimento delle matrici.
// Di fatto , si tratta di una classe finalizzata a fare da padre ad altre classi
// specializzate.

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iomanip>
#include <iostream>
#include "Vector.h"

class Matrix {
public:
// costruttori
	Matrix() {}; 	// empty constructor ; inizializza una matrice vuota
	Matrix(int rows,int cols) : rows(rows), cols(cols), data(rows, Vector(cols)) {
		std::cout << "Costruzione Matrice " << data.size() << "x" << data[0].size() << std::endl;	
		/*for(int i = 0 ; r();i++) {
			for(int j = 0 ; j < c();j++)
				std::cout << &(data[i](j)) << "\t";
			std::cout << std::endl;
		}*/
	}; 
	Matrix(std::vector<Vector> & data)
	:
	data(data),
	rows(data.size()),
	cols(data[0].size())
	{
	// Interoperabilitá con vettori di vettori
	// costruisce un oggetto a partire da un vettore di vettori
	}
	
	// operazioni elementari tra matrici
	// Get elemento
	float & operator() (int row,int col) {
		return data[row](col);
	}
	// Get riga - shortened
	Vector & operator()(int row) {
		return data[row];
	}
	
	// Get riga
	Vector & row(int row)  {
		return (*this)(row);
	}
	
	// Get col 
	Vector column(int col) {
		Vector ret(r());
		for(int i = 0 ; i < r();i++)
			ret(i) = (*this)(i,col);
		return ret;
	}
	
	// (***) dot rappresenta una funzione polimorfa che é in grado
	// di gestire entrambi i prodotti
	Vector dot(Vector & other) {
		// prodotto matrice-vettore
		Vector ret(r());					// inizializzo un vettore con tanti elementi quanto il numero di righe
		for(int i = 0;i < r();i++)
		{
			ret(i) = (*this)(i).dot(other);	// sfrutto l'operazione di prodotto interno definita nel vettore
		}
		return ret;
	}
	
	Matrix dot(Matrix & other) {
		// prodotto matrice-matrice
		Matrix ret(r(), other.c()); 
		for(int i = 0 ; i < r();i++)
			for(int j = 0 ; j < other.c();j++) {
				Vector colonna = other.column(j);
				ret(i,j) = (*this)(i).dot( colonna );
			}
		return ret;
	}
	
	// Get numero di righe
	const int r() const {return rows;}
	const int c() const {return cols;}
	
	// output
	
	void print() {
		Matrix & A = (*this);
		std::cout << std::endl;
		for(int i = 0; i < r();i++)
		{
			for(int j = 0 ; j < c();j++)
			{
				std::cout << std::setw(15) << A(i,j) ;
			}
				std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	
protected:
	std::vector<Vector> data;
	int rows;
	int cols;
};

#endif