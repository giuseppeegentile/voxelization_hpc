// classe SymmetricMatrix
// discende da matrix e contiene metodi che possono essere ottimizzati
#ifndef SYMMETRIC_MATRIX_H
#define SYMMETRIC_MATRIX_H

#include "Matrix.h"

class SymmetricMatrix : public Matrix {
	using Matrix::Matrix;				// eredita i costruttori da matrix
	
	// TODO: per il momento non raffiniamo nessun metodo attraverso la classe nuova 
	// ma ci limitiamo ad importare dal padre i metodi che poi verranno utilizzati.
	// Ció non vieta di modificare questa classe piu avanti introducendo metodi
	// specifici che siano ottimizzati nell'ottica che la matrice rappresentata sia simmetrica
	// TLDR: per adesso SymmetricMatrix e Matrix sono oggetti identici.
};

#endif