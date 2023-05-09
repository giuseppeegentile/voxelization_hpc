y:
	mkdir -p build
	
	c++						src/TestDefinitivi/TestPCACache.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp\
						-o  build/TestPCACachePerm5_1a6w
	