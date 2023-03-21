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

Coordinate IR::principalComponent()
{
	// Algoritmo: 
	// Applico il power method sulla matrice (X^T @ X)
	
	// repeat until convergence:
	//		w <- (X_zc^T @ X_zc) @ v
	//		v <- w / ||w||
	std::vector< std::vector<float> >M( 3 , std::vector<float> (3, 0.) );
	
	// calcolo la media campionaria
	
	Coordinate mean(0,0,0);
	
	for(int k = 0 ; k < data.size();k++) {
		mean.x = (data[k].x + mean.x * k)/(k+1);
		mean.y = (data[k].y + mean.y * k)/(k+1);
		mean.z = (data[k].z + mean.z * k)/(k+1);
	}
	
	std::cout << mean.x << " " << mean.y << " " << mean.z << std::endl;
	
	
	// calcolo la matrice di covarianza
	
	for(int i = 0 ; i < 3;i++)
	{	
		for(int j = 0; j < 3;j++)
		{
			for(int k = 0 ; k < data.size();k++)
			{
				M[i][j] += (data[k][i] - mean[i]) * ( data[k][j] - mean[j]);
			}
			std::cout << M[i][j] << "\t";
		}
			std::cout << std::endl;
	}
	
	
	
	// trovo l'autovettore piu significativo della matrice di covarianza
	
	std::vector<float> v(3,0.); v[0] = 1.;	// vettore initial guess di norma 1
	for ( int k = 0 ; k < 1000; k++)
	{
			std::vector<float> w(3,0.);
			for(int i = 0 ; i < 3;i++)
			{
				float buffer = 0.;
				for(int j = 0 ; j < 3;j++)
				{	
					buffer += M[i][j] * v[j];
				}
				w[i] = buffer;
			}
			
			// normalizzazione
			float norma = std::sqrt(w[0]*w[0] + w[1]*w[1] + w[2]*w[2]);
			for(int i = 0 ; i < 3;i++)
			{
				v[i] = w[i] / norma;
			}
			
			std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << std::endl;
			
	}
	
	Coordinate ret(v[0],v[1],v[2]);
	return ret;
}