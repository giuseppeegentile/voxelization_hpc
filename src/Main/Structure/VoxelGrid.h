/*VoxelGrid.h*/
//
//	Contiene i dati veri e propri. 
//	L'oggetto di classe VoxelGrid é contenuto all'interno della Structure, che si preoccupa di convertire le coordinate
//  in indici
#ifndef VOXELGRID_H
#define VOXELGRID_H
#include "../../Common/Coordinate.h"
#include "../Parser/IR.h"

class VoxelGrid
{
	public:
			VoxelGrid(IR & intermediateRepresentation, int precision = 32) :
			data(precision * precision * precision), precision(precision)
			{
				// IN : una rappresentazione intermedia
				// OUT: una griglia di voxel
				
				// carico i coefficienti necessari per l'accesso
				c = 1;
				b = precision;
				a = b * precision;
				
				// riempio la struttura
				//		carico la rappresentazione intermedia
				auto & V = intermediateRepresentation.getData();
				
				//		trovo le dimensioni del cubo
				minX = V[0].x;
				minY = V[0].y;
				minZ = V[0].z;
				
				maxX = V[0].x;
				maxY = V[0].y;
				maxZ = V[0].z;

				for( auto & v : V )
				{
					if( v.x < minX ) minX = v.x;
					if( v.y < minY ) minY = v.y;
					if( v.z < minZ ) minZ = v.z;
					
					if( v.x > maxX ) maxX = v.x;
					if( v.y > maxY ) maxY = v.y;
					if( v.z > maxZ ) maxZ = v.z;
				}

				// "coloro" i voxel nel cubo
				
				denX = maxX - minX;		// me li calcolo subito dal momento
				denY = maxY - minY;		// che sono uguali per ogni iterazione
				denZ = maxZ - minZ;
				
				mul_coeff_x = precision/denX;		// contiene il rapporto tra precision e den* in modo da saltare
				mul_coeff_y = precision/denY;		// una moltiplicazione
				mul_coeff_z = precision/denZ;
				
				for( auto & v : V )
				{
					int x = static_cast<int> ( (v.x - minX) * mul_coeff_x);	// ottiene la posizione relativa
					int y = static_cast<int> ( (v.y - minY) * mul_coeff_y);	// all'interno del cubo per ogni coordinata
					int z = static_cast<int> ( (v.z - minZ) * mul_coeff_z);	// ovvero un numero tra 0 e 1 che rappresenta
																			// la posizione. Moltiplicandola per la precisione
																			// e castando ad int si ottiene l'indice
																			// remark: mul_coeff_x = precisione/denX = precisione/(maxX - minX)
					data[x * a + y * b + z * c] = true;	
				}
			};
			
			inline bool operator ()(unsigned int x,			// non abbiamo bisogno di ritornare un reference
									unsigned int y,			// ci interessa solo la lettura delle proteine
									unsigned int z) 
			{ 	
				return data[x * a + y * b + z * c];
			}
	
	void print() {
		int ones = 0;
		for(int i = 0; i < data.size();i++) 
			ones += data[i];
		std::cout << ones;
	}
	
		int precision;
		int a;	
		int b;
		int c;
		std::vector<bool> data;		// la scelta di vector<bool> non é casuale
									// infatti utilizza 1 bit per ogni elemento
									// permettendoci di massimizzare il numero 
									// di dati nella cache
		float minX,minY,minZ,maxX,maxY,maxZ;
		float denX,denY,denZ;
		float mul_coeff_x,mul_coeff_y,mul_coeff_z;
	
};

#endif