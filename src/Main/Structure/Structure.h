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


/**
 * @brief Class representing the structure containing the protein.
 * 
 */
class Structure
{
	public:
		/**
		 * @brief Construct a new Structure object
		 * 
		 * @param source The Intermediate Representation of the protein that want to be represented.
		 * @param precision The coarse level of the representation. High values will represent more accurately the protein, at the cost of memory.
		 */
		Structure(IR & source,size_t precision = 32) : voxelGrid(source, precision) {
			// IN:  rappresentazione intermedia (sparsa)
			// OUT: rappresentazione finale (densa)
			
		};
		
		/**
		 * @brief Getter for the Voxel Grid object
		 * 
		 * @return VoxelGrid& The corresponding grid for this structure
		 */
		VoxelGrid & getVoxelGrid();
		

		/**
		 * @brief Getter the outcome of the access to the structure. If the position queried contain an element of the protein, will return true. False otherwise.
		 * 
		 * @param x X position requested
		 * @param y Y position requested
		 * @param z Z position requested
		 * @return true The protein is present in this coordinate
		 * @return false The protein is not present in this coordinate
		 */
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
		

		/**
		 * @brief Getter the outcome of the access to the structure by the Coordinate object. If the position queried contain an element of the protein, will return true. False otherwise.
		 * 
		 * @param c The coordinate that want to be accessed.
		 * @return true The protein is present in c
		 * @return false The protein is not present in c
		 */
		inline bool operator ()(Coordinate & c) {				
			// IN : coordinate spaziali
			// OUT: presenza della proteina
			
			return (*this)(c[0],c[1],c[2]);
		}
	private:
		VoxelGrid voxelGrid;
		
	
};

#endif