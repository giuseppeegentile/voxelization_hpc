/*IR.cpp*/
#include "IR.h"

void IR::push(float x,float y,float z)
{
	Coordinate c(x,y,z);
	data.push_back( c );
}

void IR::print()
{
	for(auto & c : data)
	{
		std::cout << "(" << c.x << "," << c.y << "," << c.z << ")" << std::endl;
	}
}