#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"
#include <fstream>
#include <omp.h>
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
	Parser protein_parser("res/2j47.pdb");
	IR & ir_protein = protein_parser.getIR();
	//ir_protein.sortData();
    
    // test casuale
	int n = 1e7;
	std::default_random_engine generator(40);
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	
	Structure structure(ir_protein, PRECISION);
    VoxelGrid & voxelGrid = structure.getVoxelGrid();
    // applico il test di copertura convessa
    //Timer tmr;
    //tmr.stop();
	/*** TEST 1***/
	// test : stimo l'area della proteina con un integrazione montecarlo
	int pieni = 0;
    double start;
    double end;
    double time = 0;
	for(int i = 0;i < n;i++)
	{
		float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	// Non conto la generazione
		float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	// dei numeri casuali nel test
		float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	//
		//tmr.restart();
        start = omp_get_wtime();
		pieni += structure(x,y,z);
        end = omp_get_wtime();
        time += end - start;
        //tmr.stop();

	}



    //std::cout << permutazione << " ; " << tmr.elapsed() <<  std::endl;	
    //std::cout << tmr.elapsed() << std::endl;	
    std::cout << time << std::endl;	
                
	return 0; /********************************/
}
