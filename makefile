y:
	mkdir -p build
	
	c++						src/TestDefinitivi/TestBoolVectorConfiguration.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp\
						-o  build/TestBoolVectorConfiguration
	