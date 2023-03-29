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
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 257);
	VoxelGrid & voxelGrid = S.getVoxelGrid();
    CovarianceMatrix cov(I);
	cov.print();
	Matrix A = cov.dot(cov);
	A.print();
	return 0;
}