#ifndef COVARIANCE_MATRIX_H
#define COVARIANCE_MATRIX_H

#include <deque>
#include <map>
#include <vector>
#include <numeric>
#include <fstream>
#include "IR.h"
#include "../../Common/LinearAlgebra/SymmetricMatrix.h"
#include "../../Common/LinearAlgebra/VectorSpaceMatrix.h"
#include "../../Common/Utilities.h"
#include <cmath>

constexpr size_t width = 3;
constexpr size_t heigth = 3;

/**
 * @brief SPD Covariance Matrix
 * Implemented only get by col, since is symmetric, is the same of getting the same row
 * 
 */
class CovarianceMatrix : public SymmetricMatrix {
    public:
		using SymmetricMatrix::SymmetricMatrix;		// eredito i costruttori da SymmetricMatrix
        CovarianceMatrix(IR &ir) : 
			SymmetricMatrix(3,3)      				// chiamo il costruttore di symmetric matrix
			{
				// IN  : una rappresentazione intermedia
				// OUT : la matrice di covarianza della rappresentazione intermedia
				// calcolo le medie campionarie
				
				float avg_x = 0.0;
				float avg_y= 0.0;
				float avg_z= 0.0;
				const size_t dim = ir.getData().size();
				for(auto &c: ir.getData()){
					avg_x += c[0];							// REMARK: per stabilitá numerica
					avg_y += c[1];							//         sarebbe meglio calcolare la media
					avg_z += c[2];							// 		   non come somma diretta ma come una successione
															//		   media(k+1) = (k)/(k+1) * media(k) + 1/(k+1) * x
				}											//		   in questo modo (k)/(k+1) ---> 1
															//		   mentre 1/(k+1) ---> 0
				avg_x /= dim;								//
				avg_y /= dim;								//		   Ma per adesso va bene cosí.
				avg_z /= dim;								//

				// ???
				std::ofstream csv_file;
				csv_file.open("before_stan.csv");
				for(auto & c : ir.getData()) {
						csv_file << c.getX() << ";" << c.getY() << ";" << c.getZ() << ";" << std::endl;
				}

				IR standardizedIR;
				
				for(int i = 0; i < ir.getData().size(); i++){
					standardizedIR.push(ir.getData()[i][0] - avg_x,
										ir.getData()[i][1] - avg_y,
										ir.getData()[i][2] - avg_z);
				}
				
				// ???
				std::ofstream after;
				after.open("after_stan.csv");

				for(auto & c : standardizedIR.getData()) {
					after << c.getX() << ";" << c.getY() << ";" << c.getZ() << ";" << std::endl;
				}
				

				// calcolo i prodotti scalari (TODO : riadattare al tipo Vector)
				std::vector<float> x = standardizedIR.getVectorBasis(0);
				std::vector<float> y = standardizedIR.getVectorBasis(1);
				std::vector<float> z = standardizedIR.getVectorBasis(2);
				float x_squared = std::inner_product(x.begin(), x.end(), x.begin(), 0.0f);
				float xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0f);
				float xz = std::inner_product(x.begin(), x.end(), z.begin(), 0.0f);
				float y_squared = std::inner_product(y.begin(), y.end(), y.begin(), 0.0f);
				float yz = std::inner_product(y.begin(), y.end(), z.begin(), 0.0f);
				float z_squared = std::inner_product(z.begin(), z.end(), z.begin(), 0.0f);

				// copio i valori nella matrice simmetrica
				data[0](0) = 1./(dim - 1) * x_squared;
				data[0](1) = data[1](0) = 1./(dim - 1) * xy;
				data[0](2) = data[2](0) = 1./(dim - 1) * xz;
				data[1](1) = 1./(dim - 1) * y_squared;
				data[1](2) = data[2](1) = 1./(dim - 1) * yz;
				data[2](2) = 1./(dim - 1) * z_squared;

			}

        float & operator()(const size_t row, const size_t column) {
		   /**
			 * @brief getter of the element row-col using (row, col)
			 * 
			 * @param  row 
			 * @param  column 
			 * @return float&			// (LUCA) Due commenti.
			 *							//				1. Rimaniamo sull utilizzo dei float anziché i double.
			 *							//				2. Fare questo getter const rende molto scomode alcune operazioni
			 *							//	Ho quindi modificato da  const double& --> a float &
			 */
            Matrix & A = (*this);
			return A(row,column);
        }
		
		// swap delle colonne
        void swapColumn(int permutation);		
		
		// metodi per la costruzione della PCA
		// autovalori
		public:
			// ritorna gli autovalori
			std::vector<float> eigenvalues();		// si sceglie di implementare il tipo di ritorno come vector<float> anziché Vector	
													// dal momento che il risultato non é da intendersi come un vettore 											// geometrico ma come una semplice collezione di valori, che mantiene la propria semantica													// anche in caso di rimescolamento
		private:
			// ritorna i coefficienti del polinomio caratteristico								// metodi ausiliari
			std::vector<float> charPoly();														// che vengono usati
																								// per ottenere gli autovalori 
			// ritorna le tre soluzioni (reali dal momento che é una matrice di covarianza)		// della matrice
			std::vector<float> thirdDegreeSolver(std::vector<float> coeffs);					
		
		public:
			VectorSpaceMatrix eigenvectors(std::vector<float> evalues);
};

#endif