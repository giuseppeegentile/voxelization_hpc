/*TestParser.cpp*/
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>

//// stack overflow
#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const { 
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

///////////////////////////////////////////////////

int main() {
	
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 257);
	VoxelGrid & voxelGrid = S.getVoxelGrid();
	
	// test casuale
	int n = 10000000;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);

	Timer tmr;
	// test : stimo l'area della proteina con un integrazione montecarlo
	int pieni = 0;
	for(int i = 0;i < n;i++)
	{
		float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;
		float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;
		float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	
		pieni += S(x,y,z);
	}
	std::cout << "samples  = " << n << ";";
	
	// é un risultato atteso che l'area decresca con l'aumentare della precisione.
	// infatti stiamo approssimando punti con cubetti, quindi piu diminuisce il cubetto
	// piu diminuisce il contributo in termini di area
	
	std::cout << "area stimata rispetto al cubo  = " << (static_cast<float>(pieni)/n) << ";";
	std::cout << "elapsed time = " << tmr.elapsed() << ";" << std::endl;
}