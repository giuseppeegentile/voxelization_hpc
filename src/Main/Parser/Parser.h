/* Parser.h */
//	Fornisce un WRAPPER per la classe IR, che é finalizzata a contenere rappresentazioni sparse
//  della posizione degli atomi. In particolare, il costruttore legge un file e costruisce,
//  all'interno della classe stessa, la rappresentazione intermedia.

#ifndef PARSER_H
#define PARSER_H
#include "IR.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>


//constexpr double armstrong_threshold = 1.6;

class Parser
{
	public:
		Parser( const char * filename ) {
			
			// IN:		nome del file da leggere
			// OUT:		costruisce la rappresentazione intermedia

			// apre il file
			std::ifstream file( filename , std::ifstream::in);
			// inizializza una stringa che andrá a contenere la riga
			std::string line;
			char type;
			int i = 0;
			for(; filename[i] != '.'; i++);
			type = filename[i+1];

			
			if(type != 'm'){ // if is not a mol2 (==> is a pdb)
				// fino a quando ci sono righe da leggere...
				while (std::getline(file, line))
				{
					if( line.substr(0,4) == "ATOM" ) // dava problemi con il solo "A"
					{
						// SOLUZIONE BRUTTA: leggo dopo il carattere 28 che sono sicuro ci stanno i double
						std::string data = line.substr(31, line.length() - 31);
						std::stringstream streamer(data);
						streamer.precision(std::numeric_limits<double>::digits10 - 1);
						double x,y,z;
						// SALVO I VALORI LOCALMENTE
						streamer >> x;
						streamer >> y;
						streamer >> z;
						// AGGIORNO LA STRUTTURA DATI
						
						//std::cout << x << "\t" << y << "\t" << z << std::endl;
						//getchar();
						intermediateRepresentation.push(x,y,z);
					}
				}
			}else{ // is a mol2 ifle
			std::cout << "mpl2" << std::endl;
				std::getline(file, line); // read the first line (which has the first "@")
				while (std::getline(file, line) && line.substr(0,1) != "@");
				// at this point I am in the first <TRIPOS>ATOM row -> start actually read atoms
				
				// read until the @<TRIPOS>BOND is reached (so until there are atoms)
				while (std::getline(file, line) && line.substr(0,1) != "@"){ 
					std::string data = line.substr(16, line.length() - 16);
					std::stringstream streamer(data);
					streamer.precision(std::numeric_limits<double>::digits10 - 1);
					double x,y,z;
					// SALVO I VALORI LOCALMENTE
					streamer >> x;
					streamer >> y;
					streamer >> z;
					// AGGIORNO LA STRUTTURA DATI
					
					//std::cout << x << "\t" << y << "\t" << z << std::endl;
					//getchar();
					intermediateRepresentation.push(x,y,z);
				}
				
			}
		}; 

	IR & getIR();
	private:
		IR intermediateRepresentation;
};

#endif