// Vector.h
// implementa un container elementare per vettori di float


#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <iostream>

class Vector {
public:
	// costruttori
	Vector(int n) : data(n),length(n) {};									// dimensional constructor
	Vector(std::vector<float> data) : data(data), length(data.size()) {};	// costruttore da vettore
	Vector(const Vector & other) : data(other.data), length(other.size()) {};   // copy constructor
	
	// operazioni elementari
		// getter / setter
		float & operator()(int i) {
			return data[i];
		}

		// ritorna il prodotto scalare con un altro elemento di tipo vector
		float dot(Vector & other) {
			float ret = 0;
			for(int i=0;i< other.size() ;i++) {
				ret += (*this)(i) * other(i);
			}
			return ret;
		}
		
	const int size() const {
		return length;
	}	

	std::vector<float> data;
	int length;
};

#endif