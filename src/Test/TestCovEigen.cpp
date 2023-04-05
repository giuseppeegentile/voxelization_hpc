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
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	I.populateNeighbours();
	for(int i = 0 ; i < I.getData().size();i++)
		{
			std::cout << i << "\t\t\t\t";
			for(int j = 0; j < I.getData()[i].ir_neighbours.size();j++) 
			{
				std::cout << I.getData()[i].ir_neighbours[j] << "\t";
			}
				std::cout << std::endl;
		}


	return 0;
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
				
				std::uniform_int_distribution<>     choose_point(0, I.getData().size() - 1);
				std::normal_distribution<double>          eps(0.0,100.0);
				
				choose_point.reset();
				eps.reset();
				
				Timer tmr;
				{
					IR & I = P.getIR();
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