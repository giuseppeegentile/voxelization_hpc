y:
	mkdir -p build
	c++ -o build/TestParser \
							src/Test/TestParser.cpp \
							src/Main/Structure/*.cpp \
							src/Main/Parser/*.cpp -O3 -march=native -Wall -pg -ffast-math
	