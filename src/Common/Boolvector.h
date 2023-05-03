// /*boolvector.h*/

//PARAMETRIC
// //
// // Fornisce un container ad alte prestazioni per salvare vettori di bit.
// // TODO : non é ancora utilizzato
// //
// #ifndef BOOLVECTOR_H
// #define BOOLVECTOR_H
// #include <vector>
// #include <iostream>
// #include <cstdlib>
// #include <immintrin.h>
// #include "config.h"

// #include <math.h>


// class BoolVector
// {
// 	public:
		
// 		BoolVector(size_t size) :
// 		//data( (size + sizeof(DATATYPE) + 1) / sizeof(DATATYPE) ,0), 
// 		data (size + (BIT - 1) / BIT ,0), 
// 		datasize(size)
// 		{
// 			//std::cout << "sizeof(datatype) = " << sizeof(DATATYPE) << "\tprecision = "  << PRECISION << std::endl;
// 				//std::cout << "dimensione in memoria:" << size << std::endl;
// 				//std::cout << "dimensione in vettore:" << data.size() << std::endl;
// 		}
		
// 		inline bool get(size_t i) {
// 			size_t cella = i >> SHIFT;
// 			//const size_t cella = i >> sizeof(DATATYPE);
// 			size_t settore = i & BIN;
// 			//const size_t settore = i & ((sizeof(DATATYPE) * byte_const) - 1);
// 			return (data[cella] & ( 1ull << settore )) ;
// 		}
		
// 		void set(size_t i, bool value) {
// 			//	TABELLA DI VERITÁ 
// 			//	old    = vecchio valore della cella
// 			//	input  = codifica one hot della posizione del settore (0-7)
// 			// 	value	old[i]	input[i]	|	new[i]
// 			//  ----------------------------+---------------
// 			//	0		0		0			|		0			// il settore non é quello selezionato: lascio invariato
// 			//	0		0		1			|		0					// il settore é quello selezionato:     lascio invariato
// 			//	0		1		0			|		1			// il settore non é quello selezionato: lascio invariato
// 			//	0		1		1			|		0				// ** il settore é quello selezionato: INVERTO
// 			//	1		0		0			|		0			// il settore non é quello selezionato: lascio invariato
// 			//	1		0		1			|		1				// ** il settore é quello selezionato: INVERTO
// 			//	1		1		0			|		1			// il settore non é quello selezionato: lascio invariato
// 			//	1		1		1			|		1					// il settore é quello selezionato:     lascio invariato
// 			//	----------------------------+---------------
			
// 			// if value == false:
// 			//		new = old & ~input 
// 			// else
// 			// 		new = (~old & input) | old
			
// 			//const int cella = i >> 5;
// 			//const int settore = i & 0x1f;
// 			//char input   = 1 << settore;
// 			//data[cella] = (value ? ((~(data[cella]) & input) | data[cella]) :  ( data[cella] & ~input ) );
// 			//data[cella] = (value ? (data[cella] | (1 << settore)) :  ( data[cella] & ~input ) );

// 			size_t cella = i >> SHIFT;
// 			//size_t cella = i / sizeof(DATATYPE);
// 			size_t settore = i & BIN;
// 			//size_t settore = i & ((sizeof(DATATYPE) * byte_const) - 1);
// 			//size_t settore = i % sizeof(DATATYPE);

// 			data[cella] = (data[cella] & ~(1ull << settore)) | (value << settore);
			
// 		}	
		
		
// 		size_t size() {return datasize;}
		
// 		std::vector<DATATYPE> data;
// 		size_t datasize;
// };

// #endif

//NON PARAMETRIC, 64 BIT VERSION
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
#include "config.h"

#include <math.h>


class BoolVector
{
	public:
		
		BoolVector(size_t size) :
		//data( (size + sizeof(DATATYPE) + 1) / sizeof(DATATYPE) ,0), 
		data (size + 63 / 64 ,0), 
		datasize(size)
		{
			//std::cout << "sizeof(datatype) = " << sizeof(DATATYPE) << "\tprecision = "  << PRECISION << std::endl;
				//std::cout << "dimensione in memoria:" << size << std::endl;
				//std::cout << "dimensione in vettore:" << data.size() << std::endl;
		}
		
		inline bool get(size_t i) {
			size_t cella = i >> 6;
			//const size_t cella = i >> sizeof(DATATYPE);
			size_t settore = i & 0x3f;
			//const size_t settore = i & ((sizeof(DATATYPE) * byte_const) - 1);
			return (data[cella] & ( 1ull << settore )) ;
		}
		
		void set(size_t i, bool value) {
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
			//data[cella] = (value ? ((~(data[cella]) & input) | data[cella]) :  ( data[cella] & ~input ) );
			//data[cella] = (value ? (data[cella] | (1 << settore)) :  ( data[cella] & ~input ) );

			size_t cella = i >> 6;
			//size_t cella = i / sizeof(DATATYPE);
			size_t settore = i & 0x3f;
			//size_t settore = i & ((sizeof(DATATYPE) * byte_const) - 1);
			//size_t settore = i % sizeof(DATATYPE);

			data[cella] = (data[cella] & ~(1ull << settore)) | (value << settore);
			
		}	
		
		
		size_t size() {return datasize;}
		
		std::vector<uint64_t> data;
		size_t datasize;
};

#endif