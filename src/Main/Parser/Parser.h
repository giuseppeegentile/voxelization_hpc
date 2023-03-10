/* Parser.h */
#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include "IR.h"

class Parser
{
	public:
		Parser( const char * filename )  : file( filename , std::ifstream::in) {
			
			// legge il file riga per riga riempiendo la struttura intermedia
			
			// fine
			
		}; 
		
	private:
		std::ifstream file;
		IR intermediateRepresentation;
};

#endif