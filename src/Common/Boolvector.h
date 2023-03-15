/*boolvector.h*/
//
// Fornisce un container ad alte prestazioni per salvare vettori di bit.
// TODO : non é ancora utilizzato
//
#ifndef BOOLVECTOR_H
#define BOOLVECTOR_H
#include <vector>
#include <iostream>
#include <cstdlib>

constexpr int MISS_HISTORY_SIZE = 256; //da spostare in file traits

class BoolVector
{
	public:
	static const int MAX_PRECISION = 350;
	static const int MAX_SIZE = MAX_PRECISION  * MAX_PRECISION  * MAX_PRECISION  / 8 + 8;
	
	BoolVector(int size) :
	//data( (size/8) + 1 ,0), 
	datasize(size)
	{
			std::cout << "dimensione in memoria:" << size << std::endl;
			//std::cout << "dimensione in vettore:" << data.size() << std::endl;
	}
	
	inline const bool get(int i) {
		auto division = div(i,8);
		int cella   = division.quot; // i / 8;
		char settore = division.rem; //i % 8;

		const int table_idx = i % MISS_HISTORY_SIZE;

		if(miss_history_table[table_idx]) __builtin_prefetch(&data[cella + table_idx], 0, 3);
		miss_history_table[table_idx] = true;
		
		return data[cella] & ( 1 << settore ) ;
	}
	
	void set(int i, bool value) {
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
		
		// if value == false:
		//		new = old & ~input 
		// else
		// 		new = (~old & input) | old
		
		int cella    = i / 8;
		char settore = i % 8;
		char input   = 1 << settore;
		data[cella] = (value? ((~(data[cella]) & input) | data[cella]) :  ( data[cella] & ~input ) );
	}	
	
	
	int size() {return datasize;}
	int datasize;
	
	//std::vector<char> data;
	char data[MAX_SIZE];
	char True  = 255;
	char False = 0;
	bool miss_history_table[MISS_HISTORY_SIZE] = { false };


};

#endif