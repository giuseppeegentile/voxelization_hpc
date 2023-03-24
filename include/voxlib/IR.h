/* IR.h */
//		Fornisce un container per la rappresentazione intermedia
// 		Accessibile poi per costruire la rappresentazione nella struttura
//		Ottimizzata
//		Implementa alcuni metodi utili che vengono utilizzati nella produzione
//		della struttura ottimizzata

#ifndef IR_H
#define IR_H

#include "Coordinate.h"
#include "Voxel_Traits.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

class IR {
	public:
		IR() {};
		void push(Coordinate & c);
		void push(float x,float y, float z);
		void print();
		Coordinate principalComponent();
		
		std::vector<Coordinate> & getData() {
			return data;
		}

		void rotateProtein(const double alpha, const double beta, const double gamma){
			//angoli in radianti
			const double rad_alpha = alpha * M_PI / 180.0;
			const double rad_beta = beta * M_PI / 180.0;
			const double rad_gamma = gamma * M_PI / 180.0;
			
			//precalcola seni e coseni
			const double cos_alpha = std::cos(rad_alpha);
			const double sin_alpha = std::sin(rad_alpha);
			const double cos_beta = std::cos(rad_beta);
			const double sin_beta = std::sin(rad_beta);
			const double cos_gamma = std::cos(rad_gamma);
			const double sin_gamma = std::sin(rad_gamma);

			// Righe della matrice di rotazione
			Coordinate rot_row1 = Coordinate(cos_beta * cos_gamma, sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma,  cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma);
			Coordinate rot_row2 = Coordinate(cos_beta * sin_gamma, sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,  cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma);
			Coordinate rot_row3 = Coordinate( - sin_beta		 , sin_alpha * cos_beta									   ,  cos_alpha * cos_beta);

			// Riga per colonna
			for(auto& point : data) {
				point = Coordinate(rot_row1 * point, rot_row2 * point, rot_row3 * point);
			}
		}


	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
		
};

#endif