echo "precision,bitconfig,time,"
for precision in 1 2 3 4 5
do
	for bitconfig in 1 2 3 4 
	do
		echo "#include \"metadata.h\" \n #define PRECISION precision_${precision} \n #define DATATYPE  tipo_${bitconfig}  \n #define BIT bit_${bitconfig} \n #define BIN bin_${bitconfig} \n #define SHIFT shift_${bitconfig} \n" > "config.h"
		cd ..
		cd ..
		make
		build/TestBoolVectorConfiguration > "temp.txt"
		timer=`sed -n '2p' temp.txt`
		echo "${precision},${bitconfig},${timer}" > automated_test.csv
		cd src
		cd Common
	done
done
