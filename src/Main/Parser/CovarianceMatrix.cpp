#include "CovarianceMatrix.hpp"



VectorSpaceMatrix CovarianceMatrix::eigenvectors(std::vector<float> evalues)
{
	// IN : una lista di autovalori
	// OUT: una matrice VectorSpaceMatrix che ha come colonne gli autovettori associati
	VectorSpaceMatrix ret;
	
	for(auto & lambda : evalues) {
		Matrix lambdaI  = Matrix::identity(3) * lambda;
		Matrix M   		= *this - lambdaI;					// M = A - lambda * I

		M.print();
				
		// stabilizzazione numerica
		std::vector<int> ordine = Utilities::argsort<float>(M.column(0));		// argsort della prima colonna
		M = M.rowWiseMapping(ordine);
		
		// riduzione in scala
		for(int i = 0; i < M.r() - 1;i++)
		{
			// stabilizzazione numerica
			std::vector<int> ordine = Utilities::argsort<float>(M.column(0));		// argsort della prima colonna
			M = M.rowWiseMapping(ordine);
			// mosse di gauss
			for(int j = i+1; j < M.r();j++)
			{
				for(int k = 0;k < M.c();k++)
				{
					M(j,k) =  M(j,k) - M(j,i) / M(i,i) * M(i,k);
				}
				std::cout << i << " " << j << " gauss moves"  << std::endl;
				M.print();
			}
		}
		
		std::vector<int> pivot(3);							// lista dei pivot, non inizializzata
		// popolo la lista dei pivot
		for(int k = 0;k < M.r();k++) {
			bool stop = false;
			int  p    = 0;
			for(int i = 0; (i < (M.r() - 1) ) && (!stop) ;i++)
			{
				if(std::abs(M(k,i)) < 1e-15)
				{
					p++;
				}
				else 
				{
					stop = true;
				}
			}
		}
		
		// costruisco l'ordine dei pivot
		pivot = Utilities::argsort<int>(pivot, false);
		
		std::cout << "Prima del riordinamento per pivot" << std::endl;
		M.print();
		// produco la matrice riordinata
		//M     = M.rowWiseMapping(pivot);
		std::cout << "Dopo il riordinamento per pivot" << std::endl;
		M.print();		
		// inizializzo il vettore di output
		Vector v(3);
		v(2) =   M(1,1);
		v(1) = - M(1,2);
		v(0) = - 1. / static_cast<double>(M(0,0)) * (M(0,1) * v(1) + M(0,2) * v(2) );
		
		//aggiungo alla matrice
		v = v/v.norm();
		ret.addVector(v);
	}
	
	std::cout << "Test identitá" << std::endl;
	for(int i = 0 ; i < ret.r();i++)
	{
		for(int j = 0 ; j < ret.r();j++) {
			std::cout <<"("<< i << "," << j << ") = " << ret.column(i).dot(ret.column(j)) << std::endl;
		}
	}
	return ret;
}

std::vector<float> CovarianceMatrix::eigenvalues()
{
	// OUT: ritorna i tre autovalori reali della matrice di covarianza
	std::vector<float> coeffs = charPoly();
	return thirdDegreeSolver(coeffs);
}
std::vector<float> CovarianceMatrix::charPoly() {
	// OUT: ritorna la collezione (a,b,c) tale che
	// 		det(A - lambda * I) = -1 * lambda^3  + a * lambda^2  + b * lambda + c
	CovarianceMatrix & A = (*this);
	float a = -1;
	float b = A(2,2)+A(1,1)+A(0,0);
	float c = (-A(1,1) -A(0,0)) * A(2,2) +
			  A(1,2)*A(2,1) +
			  A(0,2)*A(2,0) - 
			  A(0,0)*A(1,1) +
			  A(0,1)*A(1,0) ;
			  
	float d = (
				A(0,0)*A(1,1) - 
				A(0,1)*A(1,0)
			  )*A(2,2) +
			  (
				A(0,2)*A(1,0) -
				A(0,0)*A(1,2)
			  )*A(2,1) +
			  (
				A(0,1)*A(1,2) -
				A(0,2)*A(1,1)
			  )*A(2,0);
	
	std::vector<float> ret{a,b,c,d};
	return ret;
}

std::vector<float> CovarianceMatrix::thirdDegreeSolver(std::vector<float> coeffs) {
	float a = coeffs[0];
	float b = coeffs[1];
	float c = coeffs[2];
	float d = coeffs[3];
	
	// algoritmo
	float f = ((3.0 * c / a) - ((b * b) / (a * a ))) / 3.0;
	float g = (((2.0 * (b*b*b)) / (a * a * a)) - ((9.0 * b * c) / (a * a)) + (27.0 * d / a)) /27.0;
	float h = ((g * g) / 4.0 + (f * f * f) / 27.0);
	
	float i = std::sqrt( (g*g)/4.0 - h );
	float j = std::cbrt(i);
	float k = std::acos(-(g / (2 * i)));
	
	float L = j * -1;
	float M = std::cos(k / 3.0);
	float N = std::sqrt(3) * std::sin(k/3.0);
	float P = (b / (3.0 * a)) * -1;
	
	std::vector<float> x(3);
	x[0] = 2 * j * std::cos(k/3.0) - (b / (3.0 * a));
	x[1] = L * (M + N) + P;
	x[2] = L * (M - N) + P;
	
	// ordino dal piu grande al piu piccolo
	// in modo da avere come primo autovalore
	// quello associato alla componente
	// di maggiore varianza nella geometria della molecola
	
	std::vector<float> x_sorted(3);		
	std::vector<int> argsorted = Utilities::argsort(x);
	
	for(int i = 0 ; i < argsorted.size();i++)
		x_sorted[argsorted[i]] = x[i];
	
	return x_sorted;
};					


void CovarianceMatrix::swapColumn(int permutation){ //permutation must be a number from 1 to 6
	// definisco la lista delle permutazioni internamente allo swapper per 3 motivi
	//		1.	Maggiore localitá nella memoria. Siamo piu "vicini"
	//		2.	A livello semantico, non é importante che da fuori si conosca cosa sia
	//			una permutazione. L'importante é che si sia in grado di conoscerne l'indice
	//		3.  Dichiarando le permutazioni come variabili globali si generava un errore di doppia 
	//			definizione.
	//
	// Inoltre é stato cambiato il tipo da map a array.
	
	/*
	int permutations[][3] =					{	{0,1,2}, 
                                                {0,2,1},
                                                {1,0,2},
                                                {1,2,0},
                                                {2,0,1},
                                                {2,1,0}	};
	
	std::vector<int> perm{
							permutations[permutation][0],
							permutations[permutation][1],
							permutations[permutation][2]
						 };
    std::vector<double> temp = storage[0];
    storage[0] = storage[perm[0]];
    storage[perm[0]] = temp;

    temp = storage[1];
    storage[1] = storage[perm[1]];
    storage[perm[1]] = temp;

    temp = storage[2];
    storage[2] = storage[perm[2]];
    storage[perm[2]] = temp;
	*/
}