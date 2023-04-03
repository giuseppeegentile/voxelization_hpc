/*TestParser.cpp*/
#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
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
	
	// test casuale
	int n = 10000000;
	std::default_random_engine generator(40);
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	Timer tmr;
	/*** TEST 1***/
	// test : stimo l'area della proteina con un integrazione montecarlo
	int pieni = 0;
	for(int i = 0;i < n;i++)
	{
		tmr.stop();
		float x = distribution(generator) * voxelGrid.denX + voxelGrid.minX;	// Non conto la generazione
		float y = distribution(generator) * voxelGrid.denY + voxelGrid.minY;	// dei numeri casuali nel test
		float z = distribution(generator) * voxelGrid.denZ + voxelGrid.minZ;	//
		tmr.restart();
		pieni += S(x,y,z);
	}
	std::cout << "samples; " << n << "; ";
	std::cout << "area stimata rispetto al cubo; " << (static_cast<float>(pieni)/n) << "; ";
	
	// é un risultato atteso che l'area decresca con l'aumentare della precisione.
	// infatti stiamo approssimando punti con cubetti, quindi piu diminuisce il cubetto
	// piu diminuisce il contributo in termini di area
	

	/*** TEST 2
	//
	// Passeggiata aleatoria nel cubo continuo
	//
	float x = 0.5 * voxelGrid.denX + voxelGrid.minX;	// Non conto la generazione
	float y = 0.5 * voxelGrid.denY + voxelGrid.minY;	// dei numeri casuali nel test
	float z = 0.5 * voxelGrid.denZ + voxelGrid.minZ;	//
	int pieni = 0;
	for(int i = 0 ; i < n ; i++)
	{
		tmr.stop();
		x += 0.01 * ( (-1. + 2 * distribution(generator)) * voxelGrid.denX + voxelGrid.minX);	// Non conto la generazione
		y += 0.01 * ( (-1. + 2 * distribution(generator)) * voxelGrid.denY + voxelGrid.minY);	// dei numeri casuali nel test
		z += 0.01 * ( (-1. + 2 * distribution(generator)) * voxelGrid.denZ + voxelGrid.minZ);	//
		tmr.restart();
		pieni += S(x,y,z);
	}
	std::cout << pieni << std::endl; ***/
	std::cout << "elapsed time; " << tmr.elapsed() << "; " << std::endl;	
	//voxelGrid.print();
	return 0;
}