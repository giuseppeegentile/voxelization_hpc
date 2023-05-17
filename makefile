y:
	mkdir -p build
	
	c++						src/Test/TestPCACache.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp -Wall\
						-o  build/TestPCACache
	