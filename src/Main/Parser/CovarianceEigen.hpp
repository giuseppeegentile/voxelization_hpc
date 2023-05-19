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
 * Evaluates the covariance matrix of the IR and its eigen's values/vector
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
				
				for(long unsigned int i = 0; i < ir.getData().size(); i++){
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

				//std::cout << matrix << std::endl ;

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
		
		/**
		 * @brief Get the eigenvalues of the covariance matrix.
		 * 
		 * @return Eigen::VectorXd Cointaining the 3 eigenvalues in the eigen vector.
		 */
        Eigen::VectorXd eigenvalues(){
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(matrix);
			Eigen::VectorXd ret(3);
			ret << eigensolver.eigenvalues().data()[0], 
			eigensolver.eigenvalues().data()[1],

			eigensolver.eigenvalues().data()[2];
			return ret;									
		}		

		/**
		 * @brief return the eigenvectors of the covariance matrix
		 * 
		 * @return Eigen::Matrix3d The matrix with as columns the eigenvectors of the matrix, in order of 
		 * biggest eigenvalues
		 */
		Eigen::Matrix3d eigenvectors(){
			Eigen::Matrix3d ret;
			Eigen::VectorXd vals = eigenvalues();
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(matrix);
		    Eigen::MatrixXd eigenvectors = eigensolver.eigenvectors(); 

			 std::vector<std::pair<double, Eigen::Vector3d>> eigen_pairs;

			// Pair eigenvalues with eigenvectors
			for (int i = 0; i < vals.size(); i++) {
				eigen_pairs.push_back(std::make_pair(vals(i), eigenvectors.col(i)));
			}

			// Sort eigenvalue-eigenvector pairs based on eigenvalues (in descending order)
			std::sort(eigen_pairs.begin(), eigen_pairs.end(), [](const auto& lhs, const auto& rhs) {
				return lhs.first > rhs.first;
			});

			int j = 0;

			// Extract sorted eigenvectors
			for (const auto& pair : eigen_pairs) {
				Eigen::VectorXd sorted_eigenvector = pair.second;
				// std::cout << "Eigenvalue: " << pair.first << std::endl;
				// std::cout << "Eigenvector: \n" << sorted_eigenvector << std::endl;
				ret.col(j) = sorted_eigenvector;
				 j++;
			}

			return ret;	
		}


		/**
		 * @brief Perform the projection on the space with minimized variance.
		 * 
		 * @param permutation 
		 */
		void principalComponentProjection(int permutation) {
			// calcola gli autovettori
			auto M = eigenvectors();

			// std::cout << "printing eigenvalues matrix: " << std::endl;
			// std::cout << M << std::endl;

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