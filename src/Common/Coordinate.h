/*Coordinate.h*/

//
//	Implementa un container generico di coordinate tridimensionale
//	verosimilmente verrá usato nella fase di parsing e assemblaggio della struttura
//	É stato inserito come oggetto Common per questo motivo
//

#ifndef COORDINATE_H
#define COORDINATE_H
#include <cmath>
#include <stdexcept>
#include <vector>

class Coordinate {
	public:
		Coordinate( float x, float y, float z )	:  x(x) , y(y) , z(z) {};
		Coordinate(const std::vector<float> &v) : x(v[0]), y(v[1]), z(v[2]) {};

		/**
		 * @brief Distance introduced by norm from this coordinate to the other
		 * 
		 * @param other the coordinate to calculate the norm respect with this object
		 * @return float the distance
		 */
		float d(Coordinate & other){
			return std::sqrt( (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) + (z-other.z)*(z-other.z)  );
		}
		
		Coordinate operator*(float scalar) const {
			return Coordinate(x * scalar, y * scalar, z * scalar);
		}
		

		// transpose(this) * other
		//element-wise multiplication
		float operator*(Coordinate& other) const {
			return  x * other.x + y * other.y + z * other.z;
		}

		Coordinate operator + (Coordinate & other) const {
			return Coordinate(x + other.x, y + other.y, z + other.z);
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
		
		const float getX() const {return x;}
		const float getY() const {return y;}
		const float getZ() const {return z;}

		const float setX(const float x_) {return x = x_;}
		const float setY(const float y_) {return y = y_;}
		const float setZ(const float z_) {return z = z_;}

		const float& operator[](int i) {
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
			throw std::out_of_range("Coordinate out of range 0-2");
		}

	private:
		float x;	
		float y;
		float z;



};
#endif