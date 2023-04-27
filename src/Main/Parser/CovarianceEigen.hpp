#ifndef COVARIANCE_EIGEN_H
#define COVARIANCE_EIGEN_H

#include <deque>
#include <map>
#include <vector>
#include <numeric>
#include <fstream>
#include "IR.h"
#include "../../Common/Utilities.h"
#include <cmath>
#include <eigen3/Eigen/Dense>


/**
 * @brief SPD Covariance Matrix
 * Implemented only get by col, since is symmetric, is the same of getting the same row
 * 
 */
class CovarianceEigen{
    public:
		CovarianceEigen(IR & ir) :
			ir(ir)
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
				
				//std::cout << "Sample mean = " << avg_x << "," << avg_y << "," << avg_z << std::endl; 
	
				IR standardizedIR;
				
				for(int i = 0; i < ir.getData().size(); i++){
					standardizedIR.push(ir.getData()[i][0] - avg_x,
										ir.getData()[i][1] - avg_y,
										ir.getData()[i][2] - avg_z);
				}

				// calcolo i prodotti scalari (TODO : riadattare al tipo Vector)
				std::vector<double> x = standardizedIR.getVectorBasis(0);
				std::vector<double> y = standardizedIR.getVectorBasis(1);
				std::vector<double> z = standardizedIR.getVectorBasis(2);
				
				matrix << std::inner_product(x.begin(), x.end(), x.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(x.begin(), x.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(x.begin(), x.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),
						  std::inner_product(x.begin(), x.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(y.begin(), y.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(y.begin(), y.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),
						  std::inner_product(x.begin(), x.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(y.begin(), y.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(z.begin(), z.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.);



				// Eigen::Matrix3d chole(matrix.ldlt().matrixU());

				// std::cout << chole.row(0) << std::endl;
				// std::cout << chole.row(1) << std::endl;
				// std::cout << chole.row(2) << std::endl;
				//Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(matrix);
				
				// std::cout << eigensolver.eigenvalues() << std::endl;
				// std::cout << eigensolver.eigenvectors() << std::endl;
				
			}
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
        double & operator()(const size_t row, const size_t column) {
			return matrix(row,column);
        }
		
		// metodi per la costruzione della PCA
		// autovalori
		
        // ritorna gli autovalori
		// si sceglie di implementare il tipo di ritorno come vector<double> anziché Vector	
													// dal momento che il risultato non é da intendersi come un vettore
													// geometrico ma come una semplice collezione di valori, che mantiene la propria semantica
        std::vector<double> eigenvalues(){
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(matrix);
			std::vector<double> ret = {eigensolver.eigenvalues().data()[0],
										eigensolver.eigenvalues().data()[1],
										eigensolver.eigenvalues().data()[2]};
			return ret;									
		}		

		Eigen::Matrix3d eigenvectors(){
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(matrix);
			return eigensolver.eigenvectors();	
		}


	void principalComponentProjection(int permutation) {
		// calcola gli autovettori
		CovarianceEigen C(*this);
		auto M = C.eigenvectors();

		// calcola la permutazione 
		Utilities::permutateByIndexMap(M, permutation);

		// applico la proiezione
		ir.project(M);
	}
		


		inline const Eigen::Matrix3d getMatrix(){return matrix;}

	private:
		Eigen::Matrix3d matrix;
		IR & ir;
};

#endif