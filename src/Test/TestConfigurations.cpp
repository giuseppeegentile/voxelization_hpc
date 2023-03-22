/*TestParser.cpp*/
#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include <fstream>

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
    const int n = 10000000;

    std::ofstream csv_file;
    csv_file.open("dataset.csv");
    csv_file << "samples; " << n << "; \n";
    Parser P("res/2j47.pdb");
    IR & I = P.getIR();
    std::vector<std::vector<int>> configurations;

    configurations.push_back({0,1,2});
    configurations.push_back({0,2,1});
    configurations.push_back({1,0,2});
    configurations.push_back({1,2,0});
    configurations.push_back({2,1,0});
    configurations.push_back({2,0,1});

    for(auto &conf: configurations){
        // 10 run per ogni configurazione
        for(int counter = 0; counter < 10; counter++){
            
            Structure S(I, 257, false, conf);    //IR & source,int precision = 32,bool optimizedCalibration = false, const std::vector<int>& conf = {}
            VoxelGrid & voxelGrid = S.getVoxelGrid();
            // test casuale

            std::default_random_engine generator(40);
            std::uniform_real_distribution<double> distribution(0.0,1.0);
            
            Timer tmr;

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

            
            csv_file << "proteina" << "-2j47" << "v" << counter << ";" << "area stimata rispetto al cubo; " << (static_cast<float>(pieni)/n) << "; " << "configurazione;" << conf.at(0)<< conf.at(1) << conf.at(2) << ";" << "tempo;" << tmr.elapsed() << ";\n";
        }
    }   
    return 0;
}    

    
	