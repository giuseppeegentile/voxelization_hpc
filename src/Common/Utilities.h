// Utilities.h
// Alcuni algoritmi che possono essere utilizzati
#ifndef UTILITIES_H
#define UTILITIES_H

class Utilities {
	public:
	template<typename T>
	static std::vector<int> argsort(const std::vector<T> & x, bool dec = true)
	{
		std::vector<int> argsorted(x.size());
		for(int i = 0; i < x.size();i++)
		{
			for(int j = 0 ; j < x.size();j++)
			{
				if(	(dec? x[i] < x[j] : x[i] > x[j]) )
						argsorted[i] += 1;
			}
		}

		for( int i = 0 ; i < argsorted.size();i++)
		{
			for(int j = i+1; j < argsorted.size();j++)
			{
					if( argsorted[i] == argsorted[j])
							argsorted[j]++;
			}
		}
		return argsorted;
	}
};

#endif 