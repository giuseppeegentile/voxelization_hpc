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
			// Vai fino a dove non trovo ATOM, ovvero primo carattere A
			while (std::getline(file, line) && line[0] != 'A');
			
			// Arrivato agli ATOM, leggi le coordinate 
			while(std::getline(file, line) && line[0] == 'A') {
				// SOLUZIONE BRUTTA: leggo dopo il carattere 28 che sono sicuro ci stanno i double
				std::string data = line.substr(28, line.length());
				std::stringstream streamer(data);
				float x,y,z;
				// SALVO I VALORI LOCALMENTE
				streamer >> x;
				streamer >> y;
				streamer >> z;
				// AGGIORNO LA STRUTTURA DATI
				intermediateRepresentation.push(x,y,z);
			}
			// fine	
		}; 
	
	IR & getIR();
	private:
		IR intermediateRepresentation;
};

#endif