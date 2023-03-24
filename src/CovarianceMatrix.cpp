#include "../include/voxlib/CovarianceMatrix.hpp"


void CovarianceMatrix::add(const size_t column, const std::vector<double> &vals){
    storage.insert({column, vals});
}


std::vector<double> CovarianceMatrix::getCol(const size_t col){
    return storage[col];
}


std::vector<double> CovarianceMatrix::dotProduct(const std::vector<double> &b){
    std::vector<double> ret{std::inner_product(storage[0].begin(), storage[0].end(), b.begin(), 0.0),
           std::inner_product(storage[1].begin(), storage[1].end(), b.begin(), 0.0),
           std::inner_product(storage[2].begin(), storage[2].end(), b.begin(), 0.0)};
    return ret;
}


CovarianceMatrix CovarianceMatrix::swapColumn(int permutation){ //permutation must be a number from 1 to 6
    std::vector<int> perm = permutations[permutation];
    CovarianceMatrix cov;
    cov.add(0, storage[perm[0]]);
    cov.add(1, storage[perm[1]]);
    cov.add(2, storage[perm[2]]);
    return cov;
}