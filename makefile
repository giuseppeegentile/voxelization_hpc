y:
	mkdir -p build
	c++ -o build/TestParser \
							src/Test/TestParser.cpp \
							src/Main/Structure/*.cpp \
							src/Main/Parser/*.cpp 
	