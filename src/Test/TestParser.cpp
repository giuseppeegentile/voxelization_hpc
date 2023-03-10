/*TestParser.cpp*/
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>

int main() {
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 100);
	VoxelGrid & voxelGrid = S.getVoxelGrid();
	
	// test casuale
	int n = 100000000;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);

	std::cout << clock() << std::endl;
	for(int i = 0;i < n;i++)
	{
		float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;
		float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;
		float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;		
	}
	std::cout << clock() << std::endl;	
}