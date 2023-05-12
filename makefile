y:
	mkdir -p build
	
	g++						src/TestDefinitivi/TestPCAOnPoint.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp\
						-o  build/TestPCAOnPoint
	