/* IR.h */
//		Fornisce un container per la rappresentazione intermedia
// 		Accessibile poi per costruire la rappresentazione nella struttura
//		Ottimizzata
//		Implementa alcuni metodi utili che vengono utilizzati nella produzione
//		della struttura ottimizzata

#ifndef IR_H
#define IR_H


#include "../../Common/Utilities.h"
#include "../../Common/Coordinate.h"
#include "../../Traits/Voxel_Traits.hpp"
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

constexpr int num_neighbours = 3;

class IR {
	friend Coordinate;
	public:
		IR() {};
		void push(Coordinate & c);
		void push(double x,double y, double z);
		void print();
		Coordinate principalComponent(){};
		
		std::vector<Coordinate> & getData() {
			return data;
		}

		void setValueAt(size_t i, Coordinate value) {
			data[i] = value;
		}

		std::vector<double> getVectorBasis(const int num_basis){
			std::vector<double> ret;
			for(int i = 0; i < data.size(); i++){
				ret.push_back(data[i][num_basis]);
			}
			return ret;
		}


		// void rotateProtein(const double alpha, const double beta, const double gamma){
		// 	//angoli in radianti
		// 	const double rad_alpha = alpha * M_PI / 180.0;
		// 	const double rad_beta = beta * M_PI / 180.0;
		// 	const double rad_gamma = gamma * M_PI / 180.0;
			
		// 	//precalcola seni e coseni
		// 	const double cos_alpha = std::cos(rad_alpha);
		// 	const double sin_alpha = std::sin(rad_alpha);
		// 	const double cos_beta = std::cos(rad_beta);
		// 	const double sin_beta = std::sin(rad_beta);
		// 	const double cos_gamma = std::cos(rad_gamma);
		// 	const double sin_gamma = std::sin(rad_gamma);

		// 	// Righe della matrice di rotazione
		// 	Coordinate rot_row1 = Coordinate(cos_beta * cos_gamma, sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma,  cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma);
		// 	Coordinate rot_row2 = Coordinate(cos_beta * sin_gamma, sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,  cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma);
		// 	Coordinate rot_row3 = Coordinate( - sin_beta		 , sin_alpha * cos_beta									   ,  cos_alpha * cos_beta);

		// 	// Riga per colonna
		// 	for(auto& point : data) {
		// 		point = Coordinate(rot_row1 * point, rot_row2 * point, rot_row3 * point);
		// 	}
		// }

		void project(Eigen::Matrix3d & V);


		// return vector of index of neighbours atom in the IR indexing
		void populateNeighbours();


		void sortData(){
			std::sort(data.begin(), data.end());
		}


		// Assuming all atoms have same weigth, naive approach to calculate center of mass
		Coordinate getCenterOfMass();

	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
		
};

#endif