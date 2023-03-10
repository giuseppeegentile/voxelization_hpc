/* IR.h */
//		Fornisce un container per la rappresentazione intermedia
// 		Accessibile poi per costruire la rappresentazione nella struttura
//		Ottimizzata

#ifndef IR_H
#define IR_H


#include "../../Common/Coordinate.h"
#include <vector>

class IR {
	public:
		IR() {};
		
	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
};

#endif