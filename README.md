# voxelization_hpc
Profiling log file are named with the name of the cpp's filename followed by the description of test performed.

In html's folder the doxygen documentation can be read.

Instructions to run the test without PCA and profiling<pre>
	- make\
	- build/TestBoolOnPoint
	- valgrind --tool=callgrind --instr-atstart=no --cache-sim=yes --log-file=OutputNameFile.cachegrind build/TestBoolOnPoint
</pre>

To run the test with PCA and profiling, replace the content of makefile with the following:
<pre>

y:
	mkdir -p build
	
	c++						src/Test/TestBoolOnPoint.cpp \
						src/Main/Structure/*.cpp \
						src/Main/Parser/*.cpp \
						 -O3 -march=native -fopenmp\
						-o  build/TestBoolOnPoint
	</pre>
And then: <pre>
	- make\
	- build/TestPCAOnPoint
	- valgrind --tool=callgrind --instr-atstart=no --cache-sim=yes --log-file=OutputNameFile.cachegrind build/TestPCAOnPoint
</pre>
