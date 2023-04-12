/*TestParser.cpp*/

#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"
#include <fstream>

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
	/*
	std::vector<float> x{3.0,3.0,4.0,5.,1.,5.};
	std::vector<int> argsorted = Utilities::argsort(x,false);
	for(int i = 0 ; i < x.size();i++)
		std::cout << x[i] << "\t" << argsorted[i] << std::endl;
	return 0;*/
	// leggo il file

	Parser parser_crystal("res/crystal.mol2");
	IR & ir_crystal = parser_crystal.getIR();

	// for(auto & atoms : ir_crystal.getData()){
	// 	std::cout << atoms.getX() << " " << atoms.getY()  << " "<< atoms.getZ() << std::endl;
	// }

	//ordinamento con distanza euclidea dall'origine crescente
	ir_crystal.sortData();


	//get the center of mass of the crystal
	Coordinate com_crystal = ir_crystal.getCenterOfMass();


	std::cout << "****************** sorted ***************" << std::endl;

	// for(auto & atoms : ir_crystal.getData()){
	// 	std::cout << atoms.getX() << " " << atoms.getY()  << " "<< atoms.getZ() << std::endl;
	// }
	
	Parser protein_parser("res/2j47.pdb");
	IR & ir_protein = protein_parser.getIR();
	ir_protein.sortData();
	// ir_protein.populateNeighbours();
	// for(int i = 0 ; i < ir_protein.getData().size();i++){
	// 	std::cout << i << "\t\t\t\t";
	// 	for(int j = 0; j < ir_protein.getData()[i].ir_neighbours.size();j++) {
	// 		std::cout << ir_protein.getData()[i].ir_neighbours[j] << "\t";
	// 	}
	// 	std::cout << std::endl;
	// }

	
	//precalculate the index of the points inside the sphere of 8 angstrom to not impact the performance evaluation later
	std::vector<Coordinate> sphere; //we don't know how many points will be in the sphere...

	//first add all points of the protein in the radius of 8 from the center of mass of the crystal
	//in the second loop all point of the crystal with radius of 8 will be added

	for(auto& c: ir_protein.getData()){
		if(c.d(com_crystal) < 8) { // if the distance from a point of the protein and the center of mass of the crystal is less than 8 , then add that point in the sphere
			sphere.push_back(c); // sorry luk
		}
	}
	
	for(auto& c: ir_crystal.getData()){
		if(c.d(com_crystal) < 8) {
			sphere.push_back(c); // :( 
		}
	}

	//shuffle the array in order to have a random index array
	std::shuffle(sphere.begin(), sphere.end(), std::default_random_engine(1));
	
	Structure structure(ir_protein, 1024);

	//si usa lo stesso test di prima
	std::ofstream pca_csv;
	pca_csv.open("pca_new_test.csv");


	// per ogni test
	for(int test = 0; test < 1000;test++) {
		// per ogni permutazione

		// definiamo un vettore da 0 a 5
		std::vector<int> permutazioni{0,1,2,3,4,5};
		// lo rimescoliamo casualmente
		std::minstd_rand0 generator(0);
		std::shuffle(permutazioni.begin(), permutazioni.end(),generator) ;
		for(int permutazione : permutazioni)
		{
			Timer tmr;
			{
				VoxelGrid & voxelGrid = structure.getVoxelGrid();
				CovarianceEigen cov(ir_protein);
				cov.principalComponentProjection(permutazione);
				int pieni = 0;
				// applico il test di copertura convessa
				for(auto &s: sphere)
				{
					tmr.stop();							
					tmr.restart();
					pieni += structure(s);									
				}
			}

			std::cout << permutazione << " ; " << tmr.elapsed() <<  std::endl;	
			pca_csv << permutazione << " ; " << tmr.elapsed() << std::endl;			
		}
	}
	pca_csv.close();


	return 0; /********************************/









	std::ofstream csv_output;
	csv_output.open("pca_tests.csv");

	// per ogni test
	for(int test = 0; test < 1000;test++) {
		// per ogni permutazione

		// definiamo un vettore da 0 a 5
		std::vector<int> permutazioni{0,1,2,3,4,5};
		// lo rimescoliamo casualmente
		std::minstd_rand0 generator(0);
		std::shuffle(permutazioni.begin(), permutazioni.end(),generator) ;
		for(int permutazione : permutazioni)
		{
				int pieni = 0;
				// test casuale
				int n = 100000000;
				std::minstd_rand0 generator(0);
				
				std::uniform_int_distribution<>     choose_point(0, ir_protein.getData().size() - 1);
				std::normal_distribution<double>          eps(0.0,100.0);
				
				choose_point.reset();
				eps.reset();
				
				Timer tmr;
				{
					IR & I = protein_parser.getIR();
					Structure S(I, 257);
					VoxelGrid & voxelGrid = S.getVoxelGrid();
					CovarianceEigen cov(I);
					cov.principalComponentProjection(permutazione);

					// applico il test di copertura convessa
					for(int i = 0;i < n;i++)
					{
						tmr.stop();
						int pos      = choose_point(generator);			// k <- campionato da U(0, size()-1)
						Coordinate p = I.getData()[pos];				// p <- x[k]
						Coordinate e = Coordinate(eps(generator),		// e <- campionato da N( (0,0,0) , sigma^2)
												eps(generator),		
												eps(generator)); 		
						p           += e;								// p <- p + e
						tmr.restart();
						pieni += S(p);									// access(p)
					}
				}

				std::cout << permutazione << " ; " << tmr.elapsed() <<  std::endl;	
				csv_output << permutazione << " ; " << tmr.elapsed() << std::endl;	
		}
	}

	return 0;
}