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
			VoxelGrid(IR & intermediateRepresentation, int precision = 32, bool optimizedCalibration = false, const std::vector<int> &conf = {}) :
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
				
				//Assumo che quando harcode_test true, optimized sarà false, solo per testing
				conf.empty() ? 
								(optimizedCalibration ? PCACalibration(intermediateRepresentation) : standardCalibration()) : 
								hardcodeCalibration(conf.at(0),conf.at(1),conf.at(2));	
				
				// riempio la struttura
				//		carico la rappresentazione intermedia
				auto & V = intermediateRepresentation.getData();
				
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
				
				for( auto & v : V )
				{
					int x = static_cast<int> ( (v.getX() - minX) * mul_coeff_x);	// ottiene la posizione relativa
					int y = static_cast<int> ( (v.getY() - minY) * mul_coeff_y);	// all'interno del cubo per ogni coordinata
					int z = static_cast<int> ( (v.getZ() - minZ) * mul_coeff_z);	// ovvero un numero tra 0 e 1 che rappresenta
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
					const int raggio_x =  1.6 * mul_coeff_x;
					const int raggio_y =  1.6 * mul_coeff_y;
					const int raggio_z =  1.6 * mul_coeff_z;

					// 2. per ogni punto nel rettangolo che iscrive l'ellisse
					for ( int x_it = ( x - raggio_x > 0 ? x - raggio_x : 0 ); x_it < (x + raggio_x < precision ? x + raggio_x : precision ); x_it ++  )
						for ( int y_it = ( y - raggio_y > 0 ? y - raggio_y : 0 ); y_it < (y + raggio_y < precision ? y + raggio_y : precision ); y_it ++  )
							for ( int z_it = ( z - raggio_z > 0 ? z - raggio_z : 0 ); z_it < (z + raggio_z < precision ? z + raggio_z : precision ); z_it ++  )
							{
								// controllo di essere dentro l'ellisse
								// x2/a2 + y2/b2 + z2/c2 = 1.
								double x2 = (x_it - x)*(x_it - x);
								double y2 = (y_it - y)*(y_it - y);
								double z2 = (z_it - z)*(z_it - z);
								double rx2 = raggio_x * raggio_x;
								double ry2 = raggio_y * raggio_y;
								double rz2 = raggio_z * raggio_z;
								
								if( x2 / rx2 + y2 / ry2 + z2 / rz2  < 1.0 )
									data.set( cubeVectoriIsomorphism(x_it,y_it,z_it) , true);	
							}
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
				return data.get( cubeVectoriIsomorphism(x,y,z) );
			
			}
	
	// isomorfismo
	
	int cubeVectoriIsomorphism(int x,
							   int y,
							   int z) {
								   return x * a + y * b + z * c;
							   }
	
	void hardcodeCalibration(int a_, int b_, int c_){
		a = 1;
		b = 1;
		c = 1;
		for(; 0 < a_; a_--) a *=precision;
		for(; 0 < b_; b_--) b *=precision;
		for(; 0 < c_; c_--) c *=precision;
	}


	// vecchio metodo di costruzione dell'isomorfismo
	void standardCalibration() {
		c = 1;
		b = precision;
		a = b * precision;
	}
	
	// nuovo metodo di costruzione dell'isomorfismo
	void PCACalibration(IR & intermediateRepresentation) {
		// IN : una rappresentazione intermedia
		// OUT: modifica la mappa cubo |---> vettore in modo da rendere piu favorevole l'accesso alle tasche
		
		// IDEA: Cerchiamo di sfruttare l'informazione contenuta nella geometria della molecola in modo da ottimizzare
		// la localitá nel nostro storage.
		// In particolare, se assumiamo che il ricercatore / algoritmo si muova nei punti di una Copertura Convessificata
		// é naturale considerare come la direzione in cui c'é maggiore varianza é quella che ammette naturalmente
		// una "libertá di movimento" maggiore ; vogliamo quindi che in quella direzione la localitá sia preservata il piu possibile
		// in modo da minimizzare il rischio di cache fault
		
		// +++ PSEUDO CODICE / CODICE +++
		
		//	1. calcolo la prima componente principale v
			
		Coordinate v = intermediateRepresentation.principalComponent();
		
		// calcolo il vettore w := ( ||e_i - v|| )_i
		
		std::vector<Coordinate> e; // canonical base
		e.push_back( Coordinate(1.,0.,0.));
		e.push_back( Coordinate(0.,1.,0.));
		e.push_back( Coordinate(0.,0.,1.));
		
		std::vector<float> w(canonical_size, 0.0f);
		for(int i = 0 ; i < canonical_size; i++) {
			w[i] = e[i].d(v);
		}
		
		std::cout << "vettore \t " << w[0] << " " << w[1] << " " << w[2] << std::endl;
		
		// calcolo il vettore o := argsort(w)
		
		std::vector<int> o(canonical_size,0);
		
		for(int i = 0 ; i < canonical_size;i++)
		{
			for(int j = 0 ; j < canonical_size;j++) 
			{
					o[i] += (w[i] > w[j]);
			}
		}
		
		std::cout << "vettore \t " << o[0] << " " << o[1] << " " << o[2] << std::endl;
	
		// assegno ad ogni elemento della base canonica e_i la potenza o_i
			//	a = p^o_1
			//	b = p^o_2
			//  c = p^o_3
		
		a = 1; for(int i = 0 ; i < o[0];i++) a *= precision;
		b = 1; for(int i = 0 ; i < o[1];i++) b *= precision;
		c = 1; for(int i = 0 ; i < o[2];i++) c *= precision;
		
		// JUST FOR DEBUGGING
		
	}
	
	
	void print() {
		int ones = 0;
		for(int i = 0; i < data.size();i++) {
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
		int precision;
		float minX,minY,minZ,maxX,maxY,maxZ;
		float denX,denY,denZ;
		float mul_coeff_x,mul_coeff_y,mul_coeff_z;
	
};

#endif