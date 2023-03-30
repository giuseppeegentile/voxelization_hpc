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
				
				double avg_x = 0.0;
				double avg_y= 0.0;
				double avg_z= 0.0;
				const size_t dim = ir.getData().size();
				double i = 0;
				for(auto &c: ir.getData()){
					avg_x = (i/(i+1)*avg_x + 1./(i+1)*c[0]);							// REMARK: per stabilitá numerica
					avg_y = (i/(i+1)*avg_y + 1./(i+1)*c[1]);							//         sarebbe meglio calcolare la media
					avg_z = (i/(i+1)*avg_z + 1./(i+1)*c[2]);							// 		   non come somma diretta ma come una successione
					i+=1.0;									//		   media(k+1) = (k)/(k+1) * media(k) + 1/(k+1) * x
				}											//		   in questo modo (k)/(k+1) ---> 1
															//		   mentre 1/(k+1) ---> 0
				// ???
				
				std::cout << "Sample mean = " << avg_x << "," << avg_y << "," << avg_z << std::endl; 
				
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
				std::vector<double> x = standardizedIR.getVectorBasis(0);
				std::vector<double> y = standardizedIR.getVectorBasis(1);
				std::vector<double> z = standardizedIR.getVectorBasis(2);
				
				
				
				double x_squared = 0.;
				double xy 		 = 0.;
				double xz 		 = 0.;
				double y_squared = 0.;
				double yz		 = 0.;
				double z_squared = 0.;
				
				
				for(double i = 0 ; i < x.size();i++)
				{
					x_squared   = (i/(i+1.)) * x_squared   + x[i]*x[i]*1./(i+1.);
					xy          = (i/(i+1.)) * xy          + x[i]*y[i]*1./(i+1.);
					xz          = (i/(i+1.)) * xz          + x[i]*z[i]*1./(i+1.);
					y_squared   = (i/(i+1.)) * y_squared   + y[i]*y[i]*1./(i+1.);
					yz          = (i/(i+1.)) * yz          + y[i]*z[i]*1./(i+1.);
					z_squared   = (i/(i+1.)) * z_squared   + z[i]*z[i]*1./(i+1.);
					//std::cout << x_squared << std::endl;
				}
				
				
				//x_squared = std::inner_product(x.begin(), x.end(), x.begin(), 0.0f);
				//xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0f);
				//xz = std::inner_product(x.begin(), x.end(), z.begin(), 0.0f);
				//y_squared = std::inner_product(y.begin(), y.end(), y.begin(), 0.0f);
				//yz = std::inner_product(y.begin(), y.end(), z.begin(), 0.0f);
				//z_squared = std::inner_product(z.begin(), z.end(), z.begin(), 0.0f);

				// copio i valori nella matrice simmetrica
				
				double alpha = static_cast<double>(dim)/(static_cast<double>(dim) - 1.);
				data[0](0) 				= alpha * x_squared;
				data[0](1) = data[1](0) = alpha * xy;
				data[0](2) = data[2](0) = alpha * xz;
				data[1](1) 				= alpha * y_squared;
				data[1](2) = data[2](1) = alpha * yz;
				data[2](2) 				= alpha * z_squared;

			}

        double & operator()(const size_t row, const size_t column) {
		   /**
			 * @brief getter of the element row-col using (row, col)
			 * 
			 * @param  row 
			 * @param  column 
			 * @return double&			// (LUCA) Due commenti.
			 *							//				1. Rimaniamo sull utilizzo dei double anziché i double.
			 *							//				2. Fare questo getter const rende molto scomode alcune operazioni
			 *							//	Ho quindi modificato da  const double& --> a double &
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
			std::vector<double> eigenvalues();		// si sceglie di implementare il tipo di ritorno come vector<double> anziché Vector	
													// dal momento che il risultato non é da intendersi come un vettore 											// geometrico ma come una semplice collezione di valori, che mantiene la propria semantica													// anche in caso di rimescolamento
		private:
			// ritorna i coefficienti del polinomio caratteristico								// metodi ausiliari
			std::vector<double> charPoly();														// che vengono usati
																								// per ottenere gli autovalori 
			// ritorna le tre soluzioni (reali dal momento che é una matrice di covarianza)		// della matrice
			std::vector<double> thirdDegreeSolver(std::vector<double> coeffs);					
		
		public:
			VectorSpaceMatrix eigenvectors(std::vector<double> evalues);
};

#endif