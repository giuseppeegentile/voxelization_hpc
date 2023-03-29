#include "CovarianceMatrix.hpp"


void CovarianceMatrix::add(const size_t column, const std::vector<double> &vals){
	/**
	 * @brief Add in column-major 
	 * 
	 * @param row the column where to add 
	 * @param vals the (eigen)vector to add
	 */
    storage.insert({column, vals});
}


std::vector<double> CovarianceMatrix::getCol(const size_t col){
	/**
	 * @brief Get a Row of the matrix
	 * 
	 * @param col 
	 */
    return storage[col];
}


std::vector<double> CovarianceMatrix::dotProduct(const std::vector<double> &b){
	/**
	 * 
	 * @brief Perform row-column product (Ab)
	 * 
	 * @param b vector to multiply with the matrix
	 * @param res std::vector<double> 
	 */
    std::vector<double> ret{std::inner_product(storage[0].begin(), storage[0].end(), b.begin(), 0.0f),
                            std::inner_product(storage[1].begin(), storage[1].end(), b.begin(), 0.0f),
                            std::inner_product(storage[2].begin(), storage[2].end(), b.begin(), 0.0f)};
    return ret;
}


void CovarianceMatrix::swapColumn(int permutation){ //permutation must be a number from 1 to 6
	// definisco la lista delle permutazioni internamente allo swapper per 3 motivi
	//		1.	Maggiore localitá nella memoria. Siamo piu "vicini"
	//		2.	A livello semantico, non é importante che da fuori si conosca cosa sia
	//			una permutazione. L'importante é che si sia in grado di conoscerne l'indice
	//		3.  Dichiarando le permutazioni come variabili globali si generava un errore di doppia 
	//			definizione.
	//
	// Inoltre é stato cambiato il tipo da map a array.
	
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
}