
for precision in 1 2 3 4 5
do
	for bitconfig in 1 2 3 4 5
	do
		echo "#include \"metadata.h\" \n #define PRECISION precision_${precision} \n #define DATATYPE  tipo_${bitconfig} \n" > "config.h"
		cd ..
		cd ..
		make
		build/TestBoolVectorConfiguration > "precision${precision}_bitconfig${bitconfig}.txt"
		cd src
		cd Common
	done
done
