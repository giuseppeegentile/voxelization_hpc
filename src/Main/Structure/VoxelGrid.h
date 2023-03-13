/*VoxelGrid.h*/
//
//	Contiene i dati veri e propri. 
//	L'oggetto di classe VoxelGrid é contenuto all'interno della Structure, che si preoccupa di convertire le coordinate
//  in indici
#ifndef VOXELGRID_H
#define VOXELGRID_H
#include "../../Common/BoolVector.h"
#include "../../Common/Coordinate.h"
#include "../Parser/IR.h"
#include <array>

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
					
					// TODO (***) : questa disposizione favorisce a livello di localitá
					// i cubetti che sono sulla stessa coordinata z. Sarebbe interessante
					// costruire una mappa (i,j,k) |--> (x,y,z) che favorisca in egual misura
					// tutte le possibili direzioni
					
					data.set(x * a + y * b + z * c , true);	
				}
			};
			
			inline bool operator ()(int & x,			// non abbiamo bisogno di ritornare un reference
									int & y,			// ci interessa solo la lettura delle proteine
									int & z) 
			{ 	
				// vedi TODO (***)
				// TODO (**) : sarebbe interessante fare un prefetching
				// stocastico. Ovvero si da una probabilitá p di mettere
				// una regione di spazio in cache. 
				// ad ogni accesso di un voxel nella regione, si estrae
				// con probabilitá ( (1 - p,0)  , (p,1) )
				// Se esce 1, la regione viene aggiunta in cache
				// in questo modo se una regione viene acceduta spesso
				// la probabilitá di estrarre almeno un 1 aumenta.
				// infatti
				// P( {0} ) 		= 1 - p			|
				// P( {0,0} ) 		= (1 - p)^2		|	successione decrescente
				// P( {0,0,...,0} ) = (1 - p)^n 	V
				return data.get(x * a + y * b + z);
			
			}
	
	void print() {
		int ones = 0;
		for(int i = 0; i < data.size();i++) 
			ones += data.get(i);
		std::cout << ones;
	}
	
		int precision;
		int a;	
		int b;
		int c;
		//std::vector<bool> data;		// la scelta di vector<bool> non é casuale
										// infatti utilizza 1 bit per ogni elemento
										// permettendoci di massimizzare il numero 
										// di dati nella cache
		BoolVector data;
		float minX,minY,minZ,maxX,maxY,maxZ;
		float denX,denY,denZ;
		float mul_coeff_x,mul_coeff_y,mul_coeff_z;
	
};

#endif