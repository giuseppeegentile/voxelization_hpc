/*IR.cpp*/
#include "IR.h"

void IR::push(double x,double y,double z)
{
	Coordinate c(x,y,z);
	data.push_back( c );
}

void IR::print()
{
	for(auto & c : data)
	{
		std::cout << "(" << c.getX() << "," << c.getY() << "," << c.getZ() << ")" << std::endl;
	}
}


Coordinate IR::getCenterOfMass(){ //avg of each coordinate alone
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

void IR::populateNeighbours(){
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


void IR::project(Eigen::Matrix3d & V) {
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
			Eigen::Vector3d w = u.transpose() * V;
			data[i].setX(w[0]);
			data[i].setY(w[1]);
			data[i].setZ(w[2]);
		}

}


