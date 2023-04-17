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
#include <immintrin.h>

class BoolVector
{
	public:
		
		BoolVector(long int size) :
		 
		datasize(size)
		{
				data.reserve((size + 65) / 64);
				//std::cout << "dimensione in memoria:" << size << std::endl;
				//std::cout << "dimensione in vettore:" << data.size() << std::endl;
		}
		
		inline const bool get(long int i) {
			const long int cella = i >> 6;
			uint64_t settore = i & 0x3f;
			
			return (data[cella] & ( 1ull << settore )) ;
		}
		
		void set(unsigned long int i, bool value) {
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
			
			//const int cella = i >> 5;
			//const int settore = i & 0x1f;
			//char input   = 1 << settore;
			//data[cella] ong int = (value ? ((~(data[cella]) & input) | data[cella]) :  ( data[cella] & ~input ) );
			//data[cella] = (value ? (data[cella] | (1 << settore)) :  ( data[cella] & ~input ) );
			//std::cout << i << std::endl;			
			unsigned long int cella = i >> 6;
			uint64_t settore = i & 0x3f;
			//uint64_t settore = i % 64;
			data[cella] =(data[cella] & ~(1ull << settore)) | (value << settore);
			
		}	
		
		
		long int size() {return datasize;}
		
		std::vector<uint64_t> data;
		long int datasize;
};

#endif