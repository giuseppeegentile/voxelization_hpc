/*TestParser.cpp*/

#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"
#include <fstream>
#include "../Common/config.h"
//// stack overflow
#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()), stopped_time(0.0) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return 
		std::chrono::duration_cast<second_> (clock_::now() - beg_).count() - stopped_time; 
		}
			
	void stop()  {
		stop_start = clock_::now();
	};
	void restart() {
		stopped_time += std::chrono::duration_cast<second_> ( (clock_::now() - stop_start)).count() ;
	};

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
	
	std::chrono::time_point<clock_> stop_start;
	std::chrono::time_point<clock_> stop_end;
	double stopped_time;
};


int main() {
	

	Parser parser_crystal("res/crystal.mol2");
	IR & ir_crystal = parser_crystal.getIR();

	//ordinamento con distanza euclidea dall'origine crescente
	ir_crystal.sortData();


	//get the center of mass of the crystal
	Coordinate com_crystal = ir_crystal.getCenterOfMass();


	
	Parser protein_parser("res/2j47.pdb");
	IR & ir_protein = protein_parser.getIR();
	//ir_protein.sortData();
    

	
	//precalculate the index of the points inside the sphere of 8 angstrom to not impact the performance evaluation later
	std::vector<Coordinate> sphere; //we don't know how many points will be in the sphere...

	//first add all points of the protein in the radius of 8 from the center of mass of the crystal
	//in the second loop all point of the crystal with radius of 8 will be added
	std::cout << "***************** protein \n";
	for(auto& c: ir_protein.getData()){	
		Coordinate traslation = c + com_crystal; 	
		if(traslation.d(com_crystal) < 8) { // if the distance from a point of the protein and the center of mass of the crystal is less than 8 , then add that point in the sphere
			sphere.push_back(c); // sorry luk
			//std::cout << c.getX() << " " << c.getY() << " " << c.getZ()<< std::endl;
		}
	}
	
	IR ir(sphere);
	
	Structure structure(ir, PRECISION);
	
	//VoxelGrid & voxelGrid = S.getVoxelGrid();

	std::cout << "Element in the sphere (accesses performed to the data structure): " << sphere.size() << std::endl;


	//shuffle the array in order to have a random index array
	//std::shuffle(sphere.begin(), sphere.end(), std::default_random_engine(1));
	std::sort(sphere.begin(), sphere.end());
	
	

	//si usa lo stesso test di prima
	std::ofstream pca_csv;
	pca_csv.open("pca_new_test.csv");

	int seed = 0;
	// per ogni test
	//for(int test = 0; test < 1000;test++, seed++) {
		// per ogni permutazione

		// definiamo un vettore da 0 a 5
		std::vector<int> permutazioni{ 0, 1, 2, 3, 4, 5 };
		// lo rimescoliamo casualmente
		std::minstd_rand0 generator(seed);
		std::shuffle(permutazioni.begin(), permutazioni.end(),generator) ;
		for(int permutazione : permutazioni) {
			CovarianceEigen cov(ir);
			cov.principalComponentProjection(permutazione);

			int pieni = 0;
			// applico il test di copertura convessa
			Timer tmr;
			tmr.stop();
			for(auto &s: sphere)
			{					
				tmr.restart();
				pieni += structure(s);
				tmr.stop();
			}
			//std::cout << permutazione << " ; " << tmr.elapsed() <<  std::endl;	
			pca_csv << permutazione << " ; " << tmr.elapsed() << std::endl;	
						
		}

	//}
	pca_csv.close();


	return 0; /********************************/







	return 0;
}