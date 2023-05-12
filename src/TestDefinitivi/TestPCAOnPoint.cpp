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
	Parser protein_parser("res/fakeprotein.pdb");
	IR & ir_protein = protein_parser.getIR(); //INtermediate representation of the protein, considering each cube
                                            
    
    // test casuale
	int n = 1e7;
	std::default_random_engine generator(40);
	std::uniform_real_distribution<double> distribution(-1.0,1.0);
	
	Structure structure(ir_protein, 512);
    VoxelGrid & voxelGrid = structure.getVoxelGrid();

    std::vector<int> permutazioni{0,1,2,3,4,5}; //vettore permutazioni da testare
    volatile int pieni = 0;
    int permutazione = 4;
    CovarianceEigen cov(ir_protein.getData());
    IR ir_protein_pca = cov.principalComponentProjection(permutazione);
    Structure structure_to_access(ir_protein_pca, 512);
    float maxX = structure_to_access.getVoxelGrid().maxX;
    std::cout << maxX;
    int aca = structure_to_access(1,0,0);
    std::cout << aca << std::endl;
    // CALLGRIND_START_INSTRUMENTATION;
    // for(int i = 0;i < n; i++) {
    //     //float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	
    //     float x = i % (int)maxX;
    //     pieni += structure_to_access(x, 0, 0);
    // }
    // CALLGRIND_STOP_INSTRUMENTATION;
	// CALLGRIND_DUMP_STATS;
    return 0; 
}



