y:
	mkdir -p build
	
	c++						src/Test/TestBoolOnPoint.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp\
						-o  build/TestBoolOnPoint
	