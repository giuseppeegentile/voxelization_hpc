/*TestParser.cpp*/
#include "BoolVector.h"
#include "Parser.h"
#include "Structure.h"
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


int main() {
	Parser P("res/2j47.pdb");
	IR & I = P.getIR();
	Structure S(I, 350,false);
	
	// test casuale
	int n = 100000000;
	std::minstd_rand0 generator(0);
	
	std::uniform_int_distribution<>     choose_point(0, I.getData().size() - 1);
	std::normal_distribution<double>          eps(0.0,5.0);
	
	choose_point.reset();
	eps.reset();
	
	Timer tmr;
	/*** TEST COPERTURA CONVESSIFICATA***/
	// TEST:
	// viene costruita dinamicamente la "copertura convessificata" della proteina
	// in modo da prelevare da essa i punti da analizzare. In questo modo si simula
	// il task di ricerca nei punti vicini alle tasche dal momento che l'operazione
	// di convessificazione tende a "tappare i buchi"
	// @descrizione formale
	// Sia X := {x_1, x_2, ... , x_n} la rappresentazione sparsa della proteina ottenuta dal file pdb
	// Sia eps una variabile aleatoria di distribuzione N( (0,0,0)^T , T) dove T rappresenta il parametro di diffusione
	// Definiamo come c(x_i) la variabile aleatoria cosí definita
	//    		c(x_i) = x_i + eps
	// e definiamo C(x_i) un insieme di campionamenti indipendenti distribuiti come c(x_i)
	// Data questa nozione definiamo come C(X) il seguente insieme
	// 			C(X) = U_i C(x_i)
	
	// Esempio
	//		A
	//		|  @@@  	@@@@@@@@@@@
	//		|  @@@	    @@@@@@@@@@@
	//		|  @@@@@@@@@@@
	//		|  @@@@@@@@@@@
	//		+----------------------->	
	//											|	Effetto della copertura convessificata
	//											V
	//		A   x        xxxxxxxxxxx
	//		| x@@@xx   xx@@@@@@@@@@@x
	//		|xx@@@xxxxxx@@@@@@@@@@@
	//		| x@@@@@@@@@@@xxxxx
	//		|  @@@@@@@@@@@xx
	//		+----------------------->
	//
	// Remark: per non gravare sulla cache é sufficiente costruire i punti appartenenti alla copertura in maniera dinamicamente
	// seguendo questo pseudocodice
	
	// 1. estrai un punto x_k nella rappresentazione sparsa con k distributo uniformemente tra [1,n]
	// 2. estrai un sample e dalla distribuzione eps = N( (0,0,0), sigma^2)
	// 3. costruisci p = x_k + e
	// 4. accedi a p
	
	int pieni = 0;
	for(int i = 0;i < n;i++)
	{
		tmr.stop();
		int pos      = choose_point(generator);			// k <- campionato da U(0, size()-1)
		Coordinate p = I.getData()[pos];				// p <- x[k]
		Coordinate e = Coordinate(eps(generator),		// e <- campionato da N( (0,0,0) , sigma^2)
								  eps(generator),		
								  eps(generator)); 		
		p           += e;								// p <- p + e
		tmr.restart();
		
		pieni += S(p);									// access(p)
		
		
		
	}
	
	std::cout << "samples; " << n << "; ";
	std::cout << "area stimata rispetto al cubo; " << (static_cast<float>(pieni)/n) << "; ";
	std::cout << "elapsed time; " << tmr.elapsed() << "; " << std::endl;	

	return 0;
}