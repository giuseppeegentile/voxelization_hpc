#ifndef COVARIANCE_EIGEN_H
#define COVARIANCE_EIGEN_H

#include <deque>
#include <map>
#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
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
	private: 
		//prende la num_basis colonna 
		std::vector<double> getVectorBasis(const std::vector<std::vector<double>> & arr, const int num_basis){
			std::vector<double> ret;
			for(int i = 0; i < arr.size(); i++){
				ret.push_back(arr[i][num_basis]);
			}
			return ret;
		}

    public:
		//calcola la matrice di covarianza
		CovarianceEigen(std::vector<Coordinate> & data_) : data(data_) {
			// IN  : una rappresentazione intermedia
			// OUT : la matrice di covarianza della rappresentazione intermedia
			// calcolo le medie campionarie
			
			double avg_x = 0.0;
			double avg_y= 0.0;
			double avg_z= 0.0;
			const size_t dim = data.size();
			double i = 0;
			//calcolo media campionaria
			for(auto &c: data){
				avg_x = (i/(i+1)*avg_x + 1./(i+1)*c[0]);							
				avg_y = (i/(i+1)*avg_y + 1./(i+1)*c[1]);							
				avg_z = (i/(i+1)*avg_z + 1./(i+1)*c[2]);							
				i+=1.0;									
			}											

			std::vector<std::vector<double>> stardardizedData;														
			//popolo stardardizedIR togliendo la media dai dati iniziali
			for(int i = 0; i < data.size(); i++){
				std::vector<double> standardized = {data[i][0] - avg_x, data[i][1] - avg_y, data[i][2] - avg_z};
				stardardizedData.push_back(standardized);
			}

			// calcolo i prodotti scalari 
			std::vector<double> x = getVectorBasis(stardardizedData, 0); //prendi la 0-esima colonna
			std::vector<double> y = getVectorBasis(stardardizedData, 1); //prendi la 1-esima colonna
			std::vector<double> z = getVectorBasis(stardardizedData, 2); //prendi la 2-esima colonna
			
			matrix << std::inner_product(x.begin(), x.end(), x.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(x.begin(), x.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(x.begin(), x.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),
						std::inner_product(x.begin(), x.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(y.begin(), y.end(), y.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(y.begin(), y.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),
						std::inner_product(x.begin(), x.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.), std::inner_product(y.begin(), y.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.),std::inner_product(z.begin(), z.end(), z.begin(), 0.0f)/(static_cast<double>(dim) - 1.);

			
			std::cout << matrix << std::endl;
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
			if(eigensolver.info() ==Eigen::Success) std::cout << "success" << std::endl;
			return eigensolver.eigenvectors();	
		}


		std::vector<Coordinate> principalComponentProjection(int permutation) {
			// calcola gli autovettori
			auto M = eigenvectors();

			// calcola la permutazione 
			Utilities::permutateByIndexMap(M, permutation);

			// applico la proiezione
			return project(M);
		}
			

		
		std::vector<Coordinate> project(Eigen::Matrix3d & V) {
			// IN : una matrice che rappresenta nei suoi vettori colonna la base di uno spazio vettoriale
			// OUT: la proiezione della matrice di rappresentazione intermedia sulla base indotta dalla matrice in input

			// calcoliamo la media
			Coordinate mean(0.0f, 0.0f, 0.0f);
			std::vector<double> sum(3, 0.0f);
			for (const auto& point : data) {
				sum[0] += point.getX();
				sum[1] += point.getY();
				sum[2] += point.getZ();
			}

			const double numPoints = static_cast<double>(data.size());
			mean.setX(sum[0] / numPoints);
			mean.setY(sum[1] / numPoints);
			mean.setZ(sum[2] / numPoints);

			// rimuoviamo la media
			for(int i = 0 ; i < data.size();i++)
			{
				data[i] = data[i] - mean;	
			}

			// calcoliamo le nuove posizioni
			// per ogni vettore u_i nella rappresentazione centrata
			for( int i = 0 ; i < data.size(); i++) 
			{
				// calcolo il prodotto w_i = u_i^T V dove V é la matrice degli autovettori
				Eigen::Vector3d u( data[i].getX(), data[i].getY(), data[i].getZ()  );
				Eigen::Vector3d w = V * u;
				data[i].setX(w[0]);
				data[i].setY(w[1]);
				data[i].setZ(w[2]);
			}
			return data;
		}



		inline const Eigen::Matrix3d getMatrix(){return matrix;}

	private:
		Eigen::Matrix3d matrix; //matrice di covarianza
		std::vector<Coordinate> & data;
};

#endif