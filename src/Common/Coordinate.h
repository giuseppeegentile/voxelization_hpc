/*Coordinate.h*/

//
//	Implementa un container generico di coordinate tridimensionale
//	verosimilmente verrá usato nella fase di parsing e assemblaggio della struttura
//	É stato inserito come oggetto Common per questo motivo
//

#ifndef COORDINATE_H
#define COORDINATE_H
#include <cmath>

class Coordinate {
	public:
		Coordinate( float x, float y, float z )	:  x(x) , y(y) , z(z) {}
	float x;	// per adesso senza getter e setter
	float y;
	float z;


	float d(Coordinate & other){
		// IN : another coordinate
		// OUT: distance induced by norm
		return std::sqrt( (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) + (z-other.z)*(z-other.z)  );
	}
	
	Coordinate & operator += (Coordinate & other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return (*this);
	}
	
	Coordinate & operator -= (Coordinate & other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return (*this);
	}
	
	float operator[](int i) {
		// ammette l'accesso per indice
		switch(i) {
			case 0: {
					return x;
				break;
			}
			case 1: {
					return y;
				break;
			}
			case 2: {
					return z;
				break;
			}
		}
	}
	
};
#endif