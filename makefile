y:
	mkdir -p build
	
	c++						src/Test/TempTest.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -g \
						-o  build/TestCov 
	