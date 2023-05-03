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
	
    std::vector<int> permutazioni{0,1,2,3,4,5}; //vettore permutazioni da testare

    std::ofstream pca_csv;

	pca_csv.open("test_pca_performance.csv");


    for(auto permutazione: permutazioni){
        int pieni = 0;
        volatile double start;
        volatile double end;
        volatile double time = 0;
        CovarianceEigen cov(ir_protein);
        cov.principalComponentProjection(permutazione);
        for(int i = 0;i < n; i+=11)
        {
            float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x1 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y1 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z1 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x2 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y2 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z2 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x3 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y3 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z3 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x4 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y4 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z4 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x5 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y5 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z5 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x6 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y6 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z6 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	


            float x7 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y7 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z7 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	


            float x8 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y8 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z8 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	


            float x9 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y9 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z9 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	

            float x10 = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	
            float y10 = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	
            float z10 = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	


            //tmr.restart();
            start = omp_get_wtime();
            pieni += structure(x,y,z);
            pieni += structure(x1,y1,z1);
            pieni += structure(x2,y2,z2);
            pieni += structure(x3,y3,z3);
            pieni += structure(x4,y4,z4);
            pieni += structure(x5,y5,z5);
            pieni += structure(x6,y6,z6);
            pieni += structure(x7,y7,z7);
            pieni += structure(x8,y8,z8);
            pieni += structure(x9,y9,z9);
            pieni += structure(x10,y10,z10);
            end = omp_get_wtime();
            time += end - start;
            //tmr.stop();

        }
    
	    pca_csv << permutazione << " ; " << time << std::endl;		


        std::cout <<  permutazione << " ; " << time << std::endl;	
    }
	return 0; /********************************/
}

