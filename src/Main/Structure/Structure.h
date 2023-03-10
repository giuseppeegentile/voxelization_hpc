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
		Structure(IR & source,int precision = 32) : voxelGrid(source, precision) {
			// IN:  rappresentazione intermedia (sparsa)
			// OUT: rappresentazione finale (densa)
			
		};
		

	VoxelGrid & getVoxelGrid();
	
	inline bool operator ()			(float  &    x,			// non abbiamo bisogno di ritornare un reference
									 float  &    y,			// ci interessa solo la lettura delle proteine
									 float  &    z) 
			{ 	
				int i = static_cast<int> ( (x - voxelGrid.minX) / voxelGrid.denX * voxelGrid.precision);	// ottiene la posizione relativa
				int j = static_cast<int> ( (y - voxelGrid.minY) / voxelGrid.denY * voxelGrid.precision);	// all'interno del cubo per ogni coordinata
				int k = static_cast<int> ( (z - voxelGrid.minZ) / voxelGrid.denZ * voxelGrid.precision);	// ovvero un numero tra 0 e 1 che rappresenta
																											// la posizione. Moltiplicandola per la precisione
				
				
				return ( 
							(
								(i<0||i>voxelGrid.precision) &&
								(j<0||j>voxelGrid.precision) &&
								(k<0||k>voxelGrid.precision)
							)
							? 
							0 :
							voxelGrid(i,j,k)
						);
			}
	inline bool operator ()(Coordinate & c) {				// getter
			// IN : coordinate spaziali
			// OUT: presenza della proteina
			
			return (*this)(c.x,c.y,c.z);
		}
	private:
		VoxelGrid voxelGrid;
		
	
};

#endif