/*TestParser.cpp*/

#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include "../Main/Parser/CovarianceEigen.hpp"

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
    CovarianceEigen cov(I);
	
	return 0;
}