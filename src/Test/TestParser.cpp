/*TestParser.cpp*/
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>

int main() {
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 100);
	VoxelGrid & voxelGrid = S.getVoxelGrid();
	voxelGrid.print();
}