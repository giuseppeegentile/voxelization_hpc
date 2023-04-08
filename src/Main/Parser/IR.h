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
		Coordinate principalComponent();
		
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

	void project(Eigen::Matrix3d & V) {
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
			for( int i = 0 ; i < data.size();i++) 
			{
				// calcolo il prodotto w_i = u_i^T V dove V é la matrice degli autovettori
				Eigen::Vector3d u( data[i].getX(), data[i].getY(), data[i].getZ()  );
				Eigen::Vector3d w = u.transpose() * V;
				data[i].setX(w[0]);
				data[i].setY(w[1]);
				data[i].setZ(w[2]);
			}

	}


	// return vector of index of neighbours atom in the IR indexing
	void populateNeighbours(){
		// per ogni punto x nella rappresentazione intermedia
		for(int i = 0; i < data.size(); i++){
			// inizializzo un vettore delle distanze
			std::vector<double> distance(data.size());
			// inizializzo un vettore dell argsort delle distanze 
			std::vector<int> indexed_distance(data.size());
			// per ogni punto y nella rappresentazione intermedia
			for(int j = 0; j < data.size(); j++){
				// inserisco nel vettore delle distanze nella posizione associata ad y la distanza  ||x-y||
				distance[j] = (data[i].d(data[j]));
			}
			// trovo il vettore ordinante delle distanze appena calcolate
			indexed_distance = Utilities::argsort<double>(distance);

			// inizializzo un vettore che mette in classifica le posizioni nella ir in funzione dalla distanza con x
			std::vector<int> w(indexed_distance.size());
			for(int k = 0; k < indexed_distance.size(); k++){
				w[ indexed_distance[k] ] = k;
			}
			
			// mi tengo i primi 10 ad esclusione del punto stesso x che ha ovviamente distanza 0
			for(int l = 0; l < 3; l++)
				data[i].ir_neighbours.push_back(w[l + 1]);
		}

	}


	void sortData(){
		std::sort(data.begin(), data.end());
	}


	// Assuming all atoms have same weigth, naive approach to calculate center of mass
	Coordinate getCenterOfMass(){
		double sum_x = 0;
		double sum_y = 0;
		double sum_z = 0;
		// sum for each coordinate
		for(auto &d : data){
			sum_x += d.getX();
			sum_y += d.getY();
			sum_z += d.getZ();
		}
		// divide by number of points the summed coordinate
		return Coordinate(sum_x, sum_y, sum_z) / data.size();
	}

	private:
		std::vector<Coordinate> data;	// Array of structures dal momento
										// che accediamo a tutte le coordinate insieme (non ci interessa avere tutte le x
										// ad esempio)
		
};

#endif