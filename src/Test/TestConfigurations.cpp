/*TestParser.cpp*/
#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include <iostream>
#include <random>
#include <fstream>
#include <map>

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
constexpr int n = 10000000;
constexpr int RUN_PER_CONF = 10;

int main() {
    std::ofstream csv_file;
    csv_file.open("dataset.csv");
    csv_file << "samples; " << n << "; \n";
    Parser P("res/2j47.pdb");
    
    std::vector<std::vector<int>> configurations;

    configurations.push_back({0,1,2});
    configurations.push_back({0,2,1});
    configurations.push_back({1,0,2});
    configurations.push_back({1,2,0});
    configurations.push_back({2,1,0});
    configurations.push_back({2,0,1});

    std::map<std::string, std::vector<double>> rotations;
    
    rotations["no_rotation"] = {0,0,0};
    rotations["90_x"] = {90,0,0};
    rotations["90_y"] = {0,90,0};
    rotations["90_z"] = {0,0,90};
    
    rotations["45_x"] = {45,0,0};
    rotations["45_y"] = {0,45,0};
    rotations["45_z"] = {0,0,45};
    
    rotations["45_xy"] = {45,45,0};
    rotations["45_yz"] = {0,45,45};
    rotations["45_xz"] = {45,0,45};

    rotations["90_xy"] = {90,90,0};
    rotations["90_yz"] = {0,90,90};
    rotations["90_xz"] = {90,0,90};



    for(auto &rot: rotations){
        IR & I = P.getIR(); //ogni volta devo riprendere la IR perchè altrimenti ruota sulla proteina ruotata al ciclo precedente
        I.rotateProtein(rot.second[0], rot.second[1], rot.second[2]);
        for(auto &conf: configurations){
            // 10 run per ogni configurazione
            for(int counter = 0; counter < RUN_PER_CONF; counter++){
                
                Structure S(I, 257, false, conf); 
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

                
                csv_file << "proteina" << "-2j47" << "v" << counter << ";" << rot.first <<";" << "area stimata rispetto al cubo; " << (static_cast<float>(pieni)/n) << "; " << "configurazione;" << conf.at(0)<< conf.at(1) << conf.at(2) << ";" << "tempo;" << tmr.elapsed() << ";\n";
            }
        }   
    }
    csv_file.close();
    return 0;
}    

    
	