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
 // Print the CSV data and put the coordinates is path vector
    for (const auto& row : data) {
        path.push_back(Coordinate(std::stof(row.column5), std::stof(row.column6), std::stof(row.column7)));
        // std::cout << "Column 1: " << row.column1 << std::endl;
        // std::cout << "Column 2: " << row.column2 << std::endl;
        // std::cout << "Column 3: " << row.column3 << std::endl;
        // std::cout << "Column 4: " << row.column4 << std::endl;
        // std::cout << "Column 5: " << row.column5 << std::endl;
        // std::cout << "Column 6: " << row.column6 << std::endl;
        // std::cout << "Column 7: " << row.column7 << std::endl;
        // std::cout << "-------------------" << std::endl;

    }
	Structure structure(ir_protein, PRECISION);
    VoxelGrid & voxelGrid = structure.getVoxelGrid();

    //output file where the times will be written
    std::ofstream out_csv;
	out_csv.open("test_no_pca_performance_annealing.csv");

    std::cout << std::setprecision(15);
    volatile int pieni = 0;
    volatile double start;
    volatile double end;
    volatile double time = 0;
    
    //iterate over each row of the csv path 
    for(auto & row : path) { 
        volatile float x = row.getX();
        volatile float y = row.getY();
        volatile float z = row.getZ();
        start = omp_get_wtime();
        pieni += structure(x,y,z);
        end = omp_get_wtime();
        time += end - start;
    }

    out_csv << "time" << " ; " << time << std::endl;		


    std::cout <<  "time" << " ; " << time << std::endl;	
    
	return 0; /********************************/
}

