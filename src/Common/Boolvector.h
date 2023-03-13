/*boolvector.h*/
//
// Fornisce un container ad alte prestazioni per salvare vettori di bit.
// TODO : non é ancora utilizzato
//
#ifndef BOOLVECTOR_H
#define BOOLVECTOR_H
#include <vector>
#include <iostream>

class BoolVector
{
	public:
	
	static const unsigned int MAX_DIM	=  50 * 50 * 50;
	
	BoolVector(int size) :
	data((size+1)/8), datasize(size)
	{
					
	}
	
	const bool get(int i) {
		std::cout << i << "\t" << datasize << std::endl; 
		char cella   = i / 8;
		char settore = i % 8;
		return data[cella] & ( 1 << settore );
	}
	
	void set(int i, char value) {
		//	TABELLA DI VERITÁ 
		//	old    = vecchio valore della cella
		//	input  = codifica one hot della posizione del settore (0-7)
		// 	value	old[i]	input[i]	|	new[i]
		//  ----------------------------+---------------
		//	0		0		0			|		0			// il settore non é quello selezionato: lascio invariato
		//	0		0		1			|		0					// il settore é quello selezionato:     lascio invariato
		//	0		1		0			|		1			// il settore non é quello selezionato: lascio invariato
		//	0		1		1			|		0				// ** il settore é quello selezionato: INVERTO
		//	1		0		0			|		0			// il settore non é quello selezionato: lascio invariato
		//	1		0		1			|		1				// ** il settore é quello selezionato: INVERTO
		//	1		1		0			|		1			// il settore non é quello selezionato: lascio invariato
		//	1		1		1			|		1					// il settore é quello selezionato:     lascio invariato
		//	----------------------------+---------------
		
		
		// sum of products: (!v)o(!i) + v(!o)i + vo
		//					o(!v)(!i) + (!o)vi + ov 
		//					o(!v)(!i) + (!o)vi + ov 	
		char cella   = i / 8;
		char settore = i % 8;
		char input   = 1 << settore;
		data[cella] = (value == false? data[cella] & ~input : (~(data[cella]) & input) | data[cella] );
	}	
	
	
	int size() {return datasize;}
	int datasize;
	std::vector<char> data;
	
	char True  = 127;
	char False = 0;
};

#endif