/*TestParser.cpp*/

#include "../Common/LinearAlgebra/Vector.h"
#include "../Common/LinearAlgebra/Matrix.h"
#include "../Common/LinearAlgebra/SymmetricMatrix.h"

#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include "../Main/Parser/CovarianceMatrix.hpp"
#include <iostream>
#include <random>

//// stack overflow
#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()), stopped_time(0.0) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return 
		std::chrono::duration_cast<second_> (clock_::now() - beg_).count() - stopped_time; 
		}
			
	void stop()  {
		stop_start = clock_::now();
	};
	void restart() {
		stopped_time += std::chrono::duration_cast<second_> ( (clock_::now() - stop_start)).count() ;
	};

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
	
	std::chrono::time_point<clock_> stop_start;
	std::chrono::time_point<clock_> stop_end;
	double stopped_time;
};

///////////////////////////////////////////////////

int main() {
	/*
	std::vector<float> x{3.0,3.0,4.0,5.,1.,5.};
	std::vector<int> argsorted = Utilities::argsort(x,false);
	for(int i = 0 ; i < x.size();i++)
		std::cout << x[i] << "\t" << argsorted[i] << std::endl;
	return 0;*/
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 257);
	VoxelGrid & voxelGrid = S.getVoxelGrid();
    CovarianceMatrix cov(I);
	auto sol    = cov.eigenvalues();
	for(auto & s : sol)
		std::cout << s << std::endl;
	std::cout << "+------------------------------+" << std::endl;
	
	//std::vector<double> sol {430.54577095, 215.20620032,  86.91140941};
	VectorSpaceMatrix M = cov.eigenvectors(sol);
	M.print();
	
	return 0;
}