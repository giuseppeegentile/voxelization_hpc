y:
	mkdir -p build
	c++ 					src/Test/TestParser.cpp \
							src/Main/Structure/*.cpp \
							src/Main/Parser/*.cpp \
							-Wall -O3 -march=native -ffast-math \
							-o  build/TestParser 

	c++						src/Test/TestCoperturaConvessificata.cpp \
							src/Main/Structure/*.cpp \
							src/Main/Parser/*.cpp \
							-Wall -O3 -march=native -ffast-math\
							-o  build/TestCopertura 
	