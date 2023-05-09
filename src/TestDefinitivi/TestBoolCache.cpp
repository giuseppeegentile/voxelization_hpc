#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"
#include <fstream>
#include <omp.h>
#include <valgrind/callgrind.h>

int main() {
	Parser protein_parser("res/1a6w.pdb");
	IR & ir_protein = protein_parser.getIR();
	//ir_protein.sortData();
    
    // test casuale
	int n = 1e7;
	std::default_random_engine generator(40);
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	
	Structure structure(ir_protein, PRECISION);
    VoxelGrid & voxelGrid = structure.getVoxelGrid();
    
	int pieni = 0;
	CALLGRIND_START_INSTRUMENTATION;
	for(int i = 0;i < n; i++)
	{
		volatile float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
		volatile float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
		volatile float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

		pieni += structure(x,y,z);

	}
	CALLGRIND_STOP_INSTRUMENTATION;
	CALLGRIND_DUMP_STATS;

	return 0; /********************************/
}


