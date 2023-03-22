import os
import pandas as pd
f = open("result.csv", "w")
f.close()
cmd = 'build/TestParser >> result.csv'
for i in range(20):
	os.system(cmd)
data = pd.read_csv('result.csv', header=None, delimiter=";")
print(data)
mea = data.iloc[:, -2]
print(mea.mean())




# 10 volte per ogni configurazione
# proteina.pdb; configurazione [2,0,1]; tempo;

