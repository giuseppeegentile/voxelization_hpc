/* IR.h */
//		Fornisce un container per la rappresentazione intermedia
// 		Accessibile poi per costruire la rappresentazione nella struttura
//		Ottimizzata
//		Implementa alcuni metodi utili che vengono utilizzati nella produzione
//		della struttura ottimizzata

#ifndef IR_H
#define IR_H


#include "../../Common/Coordinate.h"
#include <iostream>
#include <vector>

class IR {
	public:
		IR() {};
		void push(Coordinate & c);
		void push(float x,float y, float z);
		void print();
	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
};

#endif