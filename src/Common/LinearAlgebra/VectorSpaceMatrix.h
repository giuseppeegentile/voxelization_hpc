// VectorSpaceMatrix.h
// Classe figlia di matrix adatta a contenere, come vettori colonne, 
// la base di uno spazio vettoriale.
// In particolare, si distingue dalla classe padre per via delle seguenti caratteristiche
//			1.  Indicizzazione per colonna.
//					Sia V una matrice di classe VectorSpaceMatrix, sia X una generica matrice.
//					Ci aspettiamo che V venga utilizzata per produrre una matrice X' = XV
//					Dunque, se indicizzassimo per riga, questo costituirebbe un bottleneck a livello di performance
//
//			2.	Possibilitá di costruzione dinamica, pushando una colonna alla volta
//					Dal momento che quando costruiamo la base di uno spazio vettoriale, lo facciamo
//					un elemento della base alla volta, la matrice di classe VectorSpaceMatrix ci permette
//					di popolarla in maniera dinamica attraverso il metodo
//							addVector( Vector y) :  [X] |-----> [X|y]

#ifndef VECTOR_SPACE_MATRIX_H
#define VECTOR_SPACE_MATRIX_H

class VectorSpaceMatrix : public Matrix {
	public:
		using Matrix::Matrix;					// importo i costruttori di matrix;
		// (2)
		void  addVector(Vector & y) {
			cols ++;
			rows = y.size();
			data.push_back(y);
		}
		
		// (1)
			// operazioni elementari tra matrici
			// Get elemento
			
			float & get(int row,int col) {
				return data[col](row);
			}
			
			float & operator() (int row,int col) {
				return get(row,col);
			}

			// Get riga
			Vector row(int row)  {
				Vector ret(c());
				for(int i = 0 ; i < c();i++)
					ret(i) = (*this)(row,i);
				return ret;
			}
			
			// Get col 
			Vector & column(int col) {
				return data[col];
			}		
		
};

#endif