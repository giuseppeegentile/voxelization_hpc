#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include "../Common/Coordinate.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"
#include <fstream>
#include <omp.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <valgrind/callgrind.h>
struct CSVData {
    std::string column1;
    std::string column2;
    std::string column3;
    std::string column4;
    std::string column5;
    std::string column6;
    std::string column7;
};


int main() {
	Parser protein_parser("res/2j47.pdb");
	IR & ir_protein = protein_parser.getIR();
	//ir_protein.sortData();
    
    // Simulated annealing, reading csv columns
    std::ifstream file("Paths/Annealing_path1683760222.390384.csv");
    std::string line;
    std::vector<CSVData> data;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        CSVData row;
        std::getline(ss, row.column1, ' ');
        std::getline(ss, row.column2, ' ');
        std::getline(ss, row.column3, ' ');
        std::getline(ss, row.column4, ' ');
        std::getline(ss, row.column5, ' ');
        std::getline(ss, row.column6, ' ');
        std::getline(ss, row.column7, ' ');
        data.push_back(row);
    }
    
    file.close();
    std::vector<Coordinate> path;
 // Print the CSV data
    for (const auto& row : data) {
        path.push_back(Coordinate(std::stof(row.column5), std::stof(row.column6), std::stof(row.column7)));
    }

	Structure structure(ir_protein, PRECISION);
    VoxelGrid & voxelGrid = structure.getVoxelGrid();
    std::vector<int> permutazioni{0,1,2,3,4,5}; //vettore permutazioni da testare

    std::cout << std::setprecision(15);
    int permutazione = 5;
    volatile int pieni = 0;
    
    CovarianceEigen cov(ir_protein);
    cov.principalComponentProjection(permutazione);
    
    //iterate over each row of the csv path
    CALLGRIND_START_INSTRUMENTATION;
    for(auto & row : path)
    {
        volatile float x = row.getX();
        volatile float y = row.getY();
        volatile float z = row.getZ();                
        pieni += structure(x,y,z);
    }
    CALLGRIND_STOP_INSTRUMENTATION;
    CALLGRIND_DUMP_STATS;
    
	return 0; /********************************/
}

