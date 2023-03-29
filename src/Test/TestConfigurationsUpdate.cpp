/*TestParser.cpp*/
#include "../Common/BoolVector.h"
#include "../Main/Parser/Parser.h"
#include "../Main/Structure/Structure.h"
#include "../Main/Parser/CovarianceMatrix.hpp"
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
constexpr int RUN_PER_CONF = 20;

int main() {
    std::ofstream csv_file;
    csv_file.open("dataset_updated.csv");
    csv_file << "samples; " << n << "; \n";
    csv_file << "Run per conf;" << RUN_PER_CONF <<";\n";
    Parser P("res/2j47.pdb");
    IR & I = P.getIR();

   


    for(int i = 1; i <= 6; i++){ // itera su tutte le 6 possibili configurazioni
        DataMatrix dm(I); //Rappresentazione matriciale della IR

        // Calcola la matrice di covarianza partendo dalla dm (invece che dalla IR come fatto nell'implementazione corrente):
        CovarianceMatrix cov(dm);
        
        // Cambio di coordinate lungo le direzioni principali
        VectorSpaceMatrix transformed = cov.getPCA();
        dm = transformed.project(dm);

        // i - esima configurazione seguendo l'ordine stabilito nella mappa memorizzata in CovarianceMatrix (eventualmente da mettere constexpr e spostare in Traits
        // dopo che sarà chiaro come usare modern cmake structure project...)
        transformed.swapColumns(i);

        // 20 run per ogni configurazione
        for(int counter = 0; counter < RUN_PER_CONF; counter++){
            //passo la DataMatrix proiettata lungo le PCA calcolate con CovarianceMatrix e memorizzate in VectorSpaceMatrix
            Structure S(dm, 257, false /**da rimuovere conf nel costruttore, anche se opzionale*/); 
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

            //stampa nel csv
            csv_file << "proteina" << "-2j47" << "v" << counter << ";"
                     << "permutazione" <<";" << i << ";"
                     << "area stimata rispetto al cubo; " << (static_cast<float>(pieni)/n) << "; " 
                     << "tempo;" << tmr.elapsed() << ";\n";
        
        }
    }

    csv_file.close();

    return 0;
}    

    
	