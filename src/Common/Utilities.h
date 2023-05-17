// Utilities.h
// Alcuni algoritmi che possono essere utilizzati
#ifndef UTILITIES_H
#define UTILITIES_H

#include "../Traits/Perm_Traits.hpp"
#include <eigen3/Eigen/Dense>
#include <vector>
#include <iostream>

class Utilities {
	public:
	template<typename T>
	static std::vector<int> argsort(const std::vector<T> & x, bool dec = true)
	{
		std::vector<int> argsorted(x.size());
		for(size_t i = 0; i < x.size();i++)
		{
			for(size_t j = 0 ; j < x.size();j++)
			{
				if(	(dec? x[i] < x[j] : x[i] > x[j]) )
						argsorted[i] += 1;
			}
		}

		for( size_t i = 0 ; i < argsorted.size();i++)
		{
			for(size_t j = i+1; j < argsorted.size();j++)
			{
					if( argsorted[i] == argsorted[j])
							argsorted[j]++;
			}
		}
		return argsorted;
	}


	static void permutateByIndexMap(Eigen::Matrix3d & mat, const int mapIndex){
		Eigen::PermutationMatrix<3, 3> perm; 
		perm.indices() = {permutations[mapIndex][0],permutations[mapIndex][1], permutations[mapIndex][2]};
		mat = mat * perm;
		// std::cout << "Mapping: " << mapIndex << std::endl;
		// std::cout << mat << std::endl;
	}
};

#endif 