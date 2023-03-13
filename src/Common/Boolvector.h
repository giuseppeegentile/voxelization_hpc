/*boolvector.h*/
//
// Fornisce un container ad alte prestazioni per salvare vettori di bit.
// TODO : non é ancora utilizzato
//

class BoolVector
{
	public:
	
	static const unsigned int MAX_DIM	=  200 * 200 * 200 / 8;
	
	BoolVector(int size) :
	size(size)
	{
			
	}
	
	const bool get(int i) {
		char cella   = i / 8;
		char settore = i % 8;
		return data[cella] & ( 2 << settore );
	}
	
	void set(int i, bool value) {
		char cella   = i / 8;
		char settore = i % 8;
		data[cella] =  ( value == true ? 
						data[cella] | ( 2 << settore )  // funzione logica 1 
						: 
						data[cella] & !( 2 << settore ) // funzione logica 2
						);
	}	
	
	
	int size;
	char data[ MAX_DIM ];
};