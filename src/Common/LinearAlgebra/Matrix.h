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
	Matrix() : rows(0),cols(0) {}; 	// empty constructor ; inizializza una matrice degenere
	Matrix(int rows,int cols) : rows(rows), cols(cols), data(rows, Vector(cols)) {
		std::cout << "Costruzione Matrice " << data.size() << "x" << data[0].size() << std::endl;	
	}; // costruttore banale ; inizializza una matrice vuota rows x cols
	Matrix(std::vector<Vector> & data)
	:
	data(data),
	rows(data.size()),
	cols(data[0].size())
	{} // Interoperabilitá con vettori di vettori
	   // costruisce un oggetto a partire da un vettore di vettori

	
	// operazioni elementari tra matrici
	// Get elemento
	
	virtual const float & getc(int row,int col) const {	// dichiaro come virtual in modo
		return data[row].getc(col);						// da fare il modo che i metodi che li utilizzano
	}			
	virtual float & get(int row,int col) {				// dichiaro come virtual in modo
		return data[row](col);							// da fare il modo che i metodi che li utilizzano
	}													// vengano "ricompilati" nei metodi ereditati
	virtual float & operator() (int row,int col) {		// dalle classi figlie
		return get(row,col);
	}
	
	// Get riga - shortened
	/* Vector & operator()(int row) {
		return data[row];
	}*/
	
	// Get riga
	virtual const Vector row(int row) const {
		return data[row];
	}
	
	// Get col 
	virtual const Vector column(int col) const  {
		Vector ret(r());
		for(int i = 0 ; i < r();i++)
			ret(i) = (*this).getc(i,col);
		return ret;
	}
	
	// (***) dot rappresenta una funzione polimorfa che é in grado
	// di gestire entrambi i prodotti
	virtual Vector dot(Vector & other)  {
		// prodotto matrice-vettore
		Vector ret(r());					// inizializzo un vettore con tanti elementi quanto il numero di righe
		for(int i = 0;i < r();i++)
		{
			ret(i) = (*this).row(i).dot(other);	// sfrutto l'operazione di prodotto interno definita nel vettore
		}
		return ret;
	}
	
	virtual Matrix dot(Matrix & other) {
		// prodotto matrice-matrice
		Matrix ret(r(), other.c()); 
		for(int i = 0 ; i < r();i++)
			for(int j = 0 ; j < other.c();j++) {
				const Vector & colonna = other.column(j);
				ret(i,j) = (*this).row(i).dot( colonna );
			}
		return ret;
	}
	
	// Get numero di righe
	const int r() const {return rows;}
	const int c() const {return cols;}
	
	// output
	
	virtual void print() {
		std::cout << "Printing Matrix..." << std::endl;
		for(int i = 0; i < r();i++)
		{
			for(int j = 0 ; j < c();j++)
			{
				std::cout << std::setw(15) << get(i,j) ;
			}
				std::cout << std::endl;
		}
		std::cout << "Printed." << std::endl;
	}
	
	
	// operatori
	
	virtual Matrix operator-(Matrix & other) {
		Matrix ret(r(),c());
		for(int i = 0 ; i < r();i++)
			for(int j = 0; j < c();j++)
				ret(i,j) = (*this)(i,j) - other(i,j);
		return ret;
	}
	
	Matrix operator * (float lambda) {
		Matrix ret(r(),c());
		for(int i = 0 ; i < r();i++)
			for(int j = 0; j < c();j++)
				ret(i,j) = (*this)(i,j) * lambda;
		return ret;
	}
	
	// identitá
	static Matrix identity(int n) {
		Matrix ret(n,n);
		for(int i = 0 ; i < n;i++)
			ret(i,i) = 1.;
		return ret;
	}
	
	// row-wise mapping
	virtual Matrix rowWiseMapping(std::vector<int> indici) {
		// IN : una collezione v di indici di righe
		// OUT: una nuova matrice che ha in ogni riga i-esima la riga v[i]-esima della matrice di partenza
		
		Matrix ret(indici.size(), c());
		for(int i = 0 ; i < indici.size();i++)
		{
			for(int j = 0 ; j < c();j++)
			{
				ret(i,j) = (*this).getc(indici[i],j);
			}
		}
		return ret;
	}
	
protected:
	std::vector<Vector> data;
	int rows;
	int cols;
};

#endif