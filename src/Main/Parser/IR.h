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
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

/**
 * @brief The intermediate representation of the protein read by parser.
 * 
 */
class IR {
	friend Coordinate;
	public:
		/**
		 * @brief Default constructor for a new IR object
		 * 
		 */
		IR() {};
		/**
		 * @brief Construct a new IR object by Coordinates (read by a pdb parser for ATOM rows)
		 * 
		 * @param coords The array of coordinates to be put in Intermediate Representation
		 */
		IR(std::vector<Coordinate> coords) : data(coords){};

		/**
		 * @brief Push a new coordinate in the object
		 * 
		 * @param c The coordinate to add
		 */
		void push(Coordinate & c);
		/**
		 * @brief Push a new coordinate in the object by the spatial triplet
		 * 
		 * @param x The x coordinate to add.
		 * @param y The y coordinate to add.
		 * @param z The z coordinate to add.
		 */
		void push(double x,double y, double z);

		/**
		 * @brief Print the coordinates of the intermediate representation
		 * 
		 */
		void print();
		

		/**
		 * @brief Get the Data vector of coordinate 
		 * 
		 * @return std::vector<Coordinate>& 
		 */
		std::vector<Coordinate> & getData() {
			return data;
		}

		/**
		 * @brief Get the requested column of the IR
		 * 
		 * @param num_basis The column to get. For instance, if 0, return the vector of the x coordinates. 1, return the y coordinates and 2 the z-s.
		 * @return std::vector<double> The vector of coordinate correponding to the canonical base requested.
		 */
		std::vector<double> getVectorBasis(const int num_basis){
			std::vector<double> ret;
			for(long unsigned int i = 0; i < data.size(); i++){
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


		/**
		 * @brief Apply a generic projection matrix to the coordinates.
		 * 
		 * @param V The projector matrix.
		 */
		void project(Eigen::Matrix3d & V);

		/**
		 * @brief Sort data based on euclidean distance from the origin. From smaller to bigger.
		 * 
		 */
		void sortData(){
			std::sort(data.begin(), data.end());
		}

		//Coordinate getCenterOfMass();

	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
		
};

#endif