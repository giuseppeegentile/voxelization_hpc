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

/**
 * @brief Voxelization's class building the grid.
 * 
 */
class VoxelGrid
{
	public:
			VoxelGrid(IR & intermediateRepresentation, size_t precision = 32) :
			data(precision * precision * precision), precision(precision)
			{
				
				// IN : una rappresentazione intermedia
				// OUT: una griglia di voxel
				
				// // carico i coefficienti necessari per l'accesso
				// if(optimizedCalibration) {
				// 	// chiama il metodo di costruzione dei coefficienti mediante PCA
				// 	PCACalibration(intermediateRepresentation);
				// } else {
				// 	// usa il metodo classico di costruzione coefficienti
				// 	standardCalibration();
				// }
				
				standardCalibration();
				
				// riempio la struttura
				//		carico la rappresentazione intermedia
				auto & V = intermediateRepresentation.getData();
				
				/**************************************************************************************************************/
				//		trovo le dimensioni del cubo
				minX = V[0].getX();
				minY = V[0].getY();
				minZ = V[0].getZ();
				
				maxX = V[0].getX();
				maxY = V[0].getY();
				maxZ = V[0].getZ();

				for( auto & v : V )
				{
					if( v.getX() < minX ) minX = v.getX();
					if( v.getY() < minY ) minY = v.getY();
					if( v.getZ() < minZ ) minZ = v.getZ();
					
					if( v.getX() > maxX ) maxX = v.getX();
					if( v.getY() > maxY ) maxY = v.getY();
					if( v.getZ() > maxZ ) maxZ = v.getZ();
				}

				// "coloro" i voxel nel cubo
				
				denX = maxX - minX;		// me li calcolo subito dal momento
				denY = maxY - minY;		// che sono uguali per ogni iterazione
				denZ = maxZ - minZ;
				
				mul_coeff_x = precision/denX;		// contiene il rapporto tra precision e den* in modo da saltare
				mul_coeff_y = precision/denY;		// una moltiplicazione
				mul_coeff_z = precision/denZ;
				

				/**
				 * TODO: fare inline
				*/
				for( auto & v : V )
				{
					long int x = static_cast<long int> ( (v.getX() - minX) * mul_coeff_x);	// ottiene la posizione relativa
					long int y = static_cast<long int> ( (v.getY() - minY) * mul_coeff_y);	// all'interno del cubo per ogni coordinata
					long int z = static_cast<long int> ( (v.getZ() - minZ) * mul_coeff_z);	// ovvero un numero tra 0 e 1 che rappresenta
																			// la posizione. Moltiplicandola per la precisione
																			// e castando ad int si ottiene l'indice
																			// remark: mul_coeff_x = precisione/denX = precisione/(maxX - minX)
					
					// TODO (***) : questa disposizione favorisce a livello di localitá
					// i cubetti che sono sulla stessa coordinata z. Sarebbe interessante
					// costruire una mappa (i,j,k) |--> (x,y,z) che favorisca in egual misura
					// tutte le possibili direzioni
					
					// write (x,y,z)
					
					// data.set(x * a + y * b + z * c , true);	
					
					// 1.	costruisco il cubetto di dimensione caratteristica 1.6 arm
					const long int raggio_x =  1.6 * mul_coeff_x;
					const long int raggio_y =  1.6 * mul_coeff_y;
					const long int raggio_z =  1.6 * mul_coeff_z;

					long int precision_cast = static_cast<long int>(precision);

					// 2. per ogni punto nel rettangolo che iscrive l'ellisse
					for ( long int x_it = ( x - raggio_x > 0 ? x - raggio_x : 0 ); x_it < (x + raggio_x < precision_cast ? x + raggio_x : precision_cast ); x_it ++  )
						for ( long int y_it = ( y - raggio_y > 0 ? y - raggio_y : 0 ); y_it < (y + raggio_y < precision_cast ? y + raggio_y : precision_cast ); y_it ++  )
							for (long int z_it = ( z - raggio_z > 0 ? z - raggio_z : 0 ); z_it < (z + raggio_z < precision_cast ? z + raggio_z : precision_cast ); z_it ++  )
							{
								// controllo di essere dentro l'ellisse
								// x2/a2 + y2/b2 + z2/c2 = 1.
								double x2 = (x_it - x)*(x_it - x);
								double y2 = (y_it - y)*(y_it - y);
								double z2 = (z_it - z)*(z_it - z);
								double rx2 = raggio_x * raggio_x;
								double ry2 = raggio_y * raggio_y;
								double rz2 = raggio_z * raggio_z;
								
								if( (x2 / rx2) + (y2 / ry2) + (z2 / rz2 ) < 1.0 )
									data.set( cubeVectoriIsomorphism(x_it,y_it,z_it) , true);	
								
							}
				}
			};
			
			inline bool operator ()(long int & x,			// non abbiamo bisogno di ritornare un reference
									long int & y,			// ci interessa solo la lettura delle proteine
									long int & z) 
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
				return data.get( cubeVectoriIsomorphism(x,y,z) );
			
			}
	

	/**
	 * @brief Isomorphism mapping the coordinates into the grid.
	 * 
	 * @param x The x coordinate to be mapped
	 * @param y The y coordinate to be mapped
	 * @param z The z coordinate to be mapped
	 * @return long int the position in the cube.
	 */
	long int cubeVectoriIsomorphism(long int x,
									long int y,
									long int z) {
								   return x * a + y * b + z * c;
							   }
	

	/**
	 * @brief Default isomorphism as a poly with x's coefficient squared, y's linar and z's costant.
	 * a^2 * x + b * y + z
	 * 
	 */
	void standardCalibration() {
		c = 1;
		b = precision;
		a = b * precision;
	}
	
	
	
	
	void print() {
		int ones = 0;
		for(long unsigned int i = 0; i < data.size();i++) {
			std::cout << data.get(i) ;
			ones += data.get(i);
		}
		//std::cout << ones;
	}
	
		
		int a;	
		int b;
		int c;
		//std::vector<bool> data;		// la scelta di vector<bool> non é casuale
										// infatti utilizza 1 bit per ogni elemento
										// permettendoci di massimizzare il numero 
										// di dati nella cache
		BoolVector data;
		size_t precision;
		float minX,minY,minZ,maxX,maxY,maxZ;
		float denX,denY,denZ;
		float mul_coeff_x,mul_coeff_y,mul_coeff_z;
	
};

#endif