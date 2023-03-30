// Vector.h
// implementa un container elementare per vettori di float


#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <iostream>
#include <cmath>

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
		// getter const
		const float & getc(int i) const {
			return data[i];
		}

		// ritorna il prodotto scalare con un altro elemento di tipo vector
		float dot(const Vector & other) const {
			float ret = 0;
			for(int i=0;i< other.size() ;i++) {
				ret += (*this).getc(i) * other.getc(i);
			}
			return ret;
		}
		
	const int size() const {
		return length;
	}	

	std::vector<float> data;
	int length;
	
	float norm() const {
		float ret = 0;
		for(int i = 0 ; i < size();i++)
		{
			ret += data[i] * data[i];
		}
		return std::sqrt(ret);
	}
	
	Vector operator / (float x) {
		Vector ret(size());
		for(int i = 0 ; i < size();i++)
		{
			ret(i) = data[i] / x;
		}
		return ret;
	}
	
	operator std::vector<float> () const {
		return data;
	}
};

#endif