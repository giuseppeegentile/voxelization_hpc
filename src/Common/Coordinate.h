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
		Coordinate( double x, double y, double z )	:  x(x) , y(y) , z(z) {}
	double x;	// per adesso senza getter e setter
	double y;
	double z;	
};
#endif