/*Coordinate.h*/

//
//	Implementa un container generico di coordinate tridimensionale
//	verosimilmente verrá usato nella fase di parsing e assemblaggio della struttura
//	É stato inserito come oggetto Common per questo motivo
//

#ifndef COORDINATE_H
#define COORDINATE_H
class Coordinate {
	public:
		Coordinate( float x, float y, float z )	:  x(x) , y(y) , z(z) {}
	float x;	// per adesso senza getter e setter
	float y;
	float z;	
};
#endif