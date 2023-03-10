/*Structure.h*/
//
// 	Fornisce un WRAPPER per l'accesso a un oggetto VoxelGrid incapsulato all'interno della stessa classe.
//	In questo modo l'utente puo accedere alle coordinate richieste in maniera "trasparente" senza preoccuparsi
//	di fare i calcoli sugli indici, che invece vengono automatizzati dalla classe
//
#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "../Parser/IR.h"
#include "VoxelGrid.h"

class Structure
{
	public:
		Structure(IR & source) {
			// IN:  rappresentazione intermedia (sparsa)
			// OUT: rappresentazione finale (densa)
			
			
			
		};
		
		inline bool operator ()(Coordinate & c) {							// getter
			return (*this)(c.x,c.y,c.z);
		}
		inline bool operator ()(float & x,float & y,float & z) {			// getter
			return 0;
		}
	
	private:
		VoxelGrid voxelGrid;
};

#endif