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
#include <cmath>
#include <numeric>
#include <algorithm>
class Coordinate {
	friend class IR;
	public:
		Coordinate( double x, double y, double z )	:  x(x) , y(y) , z(z) {};
		Coordinate(const std::vector<double> &v) : x(v[0]), y(v[1]), z(v[2]) {};

		/**
		 * @brief Distance introduced by norm from this coordinate to the other
		 * 
		 * @param other the coordinate to calculate the norm respect with this object
		 * @return double the distance
		 */
		double d(Coordinate & other){
			// std::vector<double> tmp = {(x-other.x), (y-other.y), (z-other.z)};
			// return std::inner_product(tmp.begin(), tmp.end(), tmp.begin(), 0.0);
			return std::sqrt( (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) + (z-other.z)*(z-other.z)  );
		}
		
		Coordinate operator*(double scalar) const {
			return Coordinate(x * scalar, y * scalar, z * scalar);
		}
		
		Coordinate operator + (Coordinate & other) const {
			return Coordinate(x + other.x, y + other.y, z + other.z);
		}

		Coordinate operator - (Coordinate & other) const {
			return Coordinate(x - other.x, y - other.y, z - other.z);
		}

		Coordinate operator / (double q) const {
			return Coordinate(x / q, y / q, z / q);
		}

		// transpose(this) * other
		//element-wise multiplication
		double operator*(Coordinate& other) const {
			return  x * other.x + y * other.y + z * other.z;
		}

		// is this smaller in norm than other ? yes -> true, no -> false
		bool operator < (Coordinate & other) const {
			// return Coordinate(x - other.x, y - other.y, z - other.z);
			std::vector<double> this_coordinate = {x, y, z};
			std::vector<double> other_coordinate = {other.x, other.y, other.z};
			return (std::inner_product(this_coordinate.begin(), this_coordinate.end(), this_coordinate.begin(), 0.0) 
										< 
					std::inner_product(other_coordinate.begin(), other_coordinate.end(), other_coordinate.begin(), 0.0) );
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
		
		const double getX() const {return x;}
		const double getY() const {return y;}
		const double getZ() const {return z;}

		const double setX(const double x_) {return x = x_;}
		const double setY(const double y_) {return y = y_;}
		const double setZ(const double z_) {return z = z_;}

		const double& operator[](int i) {
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
		double x;	
		double y;
		double z;

		
	public: // just for test 
		std::vector<int> ir_neighbours;
};
#endif
