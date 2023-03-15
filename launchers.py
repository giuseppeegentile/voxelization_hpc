import os
import subprocess
cmd1 = 'sudo rm result.csv'
os.system(cmd1)
cmd = 'build/TestParser >> result.csv'
for i in range(20):
	os.system(cmd)
