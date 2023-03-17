import os
import pandas as pd
# cmd1 = 'sudo rm result.csv'
# os.system(cmd1)
f = open("result.csv", "w")
f.close()
cmd = 'build/TestParser >> result.csv'
for i in range(20):
	os.system(cmd)
data = pd.read_csv('result.csv', header=None, delimiter=";")
print(data)
mea = data.iloc[:, -2]
print(mea.mean())
