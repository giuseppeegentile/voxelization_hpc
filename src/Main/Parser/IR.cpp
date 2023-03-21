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
		std::cout << "(" << c.getX() << "," << c.getY() << "," << c.getZ() << ")" << std::endl;
	}
}

Coordinate IR::principalComponent()
{
	// Algoritmo: 
	// Applico il power method sulla matrice (X^T @ X)
	
	// repeat until convergence:
	//		w <- (X_zc^T @ X_zc) @ v
	//		v <- w / ||w||
	
	// calcolo la media campionaria
	
	Coordinate mean(0.0f, 0.0f, 0.0f);
	std::vector<float> sum(3, 0.0f);
	/*for(size_t k = 0 ; k < data.size(); k++) {
		mean.setX((data[k].getX() + mean.getX() * k) / (k+1));
		mean.setY((data[k].getY() + mean.getY() * k) / (k+1));
		mean.setZ((data[k].getZ() + mean.getZ() * k) / (k+1));
	}*/
	for (const auto& point : data) {
		sum[0] += point.getX();
		sum[1] += point.getY();
		sum[2] += point.getZ();
	}

	const float numPoints = static_cast<float>(data.size());
	mean.setX(sum[0] / numPoints);
	mean.setY(sum[1] / numPoints);
	mean.setZ(sum[2] / numPoints);

	
	std::cout << mean.getX() << " " << mean.getY() << " " << mean.getZ() << std::endl;
	
	
	// calcolo la matrice di covarianza
	//versione 2 *************************

	std::vector<std::vector<float>> M(3, std::vector<float>(3, 0.0f));
	for (size_t k = 0; k < data.size(); k++) {
		const float x_dev = data[k].getX() - mean.getX();
		const float y_dev = data[k].getY() - mean.getY();
		const float z_dev = data[k].getZ() - mean.getZ();
		//l'unica differenza è avere for in meno, unrollati
		M[0][0] += x_dev * x_dev;
		M[0][1] += x_dev * y_dev;
		M[0][2] += x_dev * z_dev;
		M[1][0] += y_dev * x_dev;
		M[1][1] += y_dev * y_dev;
		M[1][2] += y_dev * z_dev;
		M[2][0] += z_dev * x_dev;
		M[2][1] += z_dev * y_dev;
		M[2][2] += z_dev * z_dev;
	}


	//versione 1 *************************
	/*for(int i = 0 ; i < 3;i++)
	{	
		for(int j = 0; j < 3;j++)
		{
			for(size_t k = 0 ; k < data.size();k++)
			{
				M[i][j] += (data[k][i] - mean[i]) * ( data[k][j] - mean[j]);
			}
			std::cout << M[i][j] << "\t";
		}
			std::cout << std::endl;
	}*/
	
	
	
	// trovo l'autovettore piu significativo della matrice di covarianza
	
	std::vector<float> v = {1, 0 ,0}; // vettore initial guess di norma 1
	for (int k = 0 ; k < maxiter_power; k++)
	{
		std::vector<float> w;
		w.reserve(canonical_size);
		for(int i = 0 ; i < canonical_size;i++)
		{
			float buffer = 0.0f;
			for(int j = 0 ; j < canonical_size; j++) {	
				buffer += M[i][j] * v[j];
			}
			//w[i] = buffer;
			w.push_back(buffer);
		}
		
		// normalizzazione
		//float norma = std::sqrt(w[0]*w[0] + w[1]*w[1] + w[2]*w[2]);
		float norma = std::sqrt(std::inner_product(w.begin(), w.end(), w.begin(), 0.0f));
		// for(int i = 0; i < canonical_size; i++)
		// 	v[i] = w[i] / norma;
		for(float& x: w) x /= norma;
		std::cout << v[0] << "\t" << v[1] << "\t" << v[2] << std::endl;
	}
	
	Coordinate ret(v);
	return ret;
}