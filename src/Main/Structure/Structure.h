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
		Structure(IR & source,size_t precision = 32,bool optimizedCalibration = false) : voxelGrid(source, precision, optimizedCalibration) {
			// IN:  rappresentazione intermedia (sparsa)
			// OUT: rappresentazione finale (densa)
			
		};
		

		VoxelGrid & getVoxelGrid();
		
		inline bool operator ()( float     x,			// non abbiamo bisogno di ritornare un reference
								 float     y,			// ci interessa solo la lettura delle proteine
								 float     z) 
		{ 	
			// IN : coordinate spaziali
			// OUT: presenza della proteina
			long int i = static_cast<long int> ( (x - voxelGrid.minX) * voxelGrid.mul_coeff_x);	// ottiene la posizione relativa
			long int j = static_cast<long int> ( (y - voxelGrid.minY) * voxelGrid.mul_coeff_y);	// all'interno del cubo per ogni coordinata
			long int k = static_cast<long int> ( (z - voxelGrid.minZ) * voxelGrid.mul_coeff_z);	// ovvero un numero tra 0 e 1 che rappresenta
																						// la posizione. Moltiplicandola per la precisione
			bool condition = (
								((i<0)||(i>=voxelGrid.precision)) ||
								((j<0)||(j>=voxelGrid.precision)) ||
								((k<0)||(k>=voxelGrid.precision))
							);
			
			return (condition ? 0 : voxelGrid(i,j,k));
		}
		
		inline bool operator ()(Coordinate & c) {				// getter
			// IN : coordinate spaziali
			// OUT: presenza della proteina
			
			return (*this)(c[0],c[1],c[2]);
		}
	private:
		VoxelGrid voxelGrid;
		
	
};

#endif