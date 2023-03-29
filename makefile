y:
	mkdir -p build
	
	c++						src/Test/TestCov.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -ffast-math\
						-o  build/TestCov
	