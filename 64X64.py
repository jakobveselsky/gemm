import numpy as np
from statistics import mean

File_data = np.loadtxt("64x64.txt", dtype='str')
x86 = []
unvec = []
omp = []
index = 1

for i in File_data:
    if(index % 3) == 0:
        unvec.append(float(i[0][0:7]))
    elif(index % 2):
        omp.append(float(i[0][0:7]))
    else:
        x86.append(float(i[0][0:7]))
    index += 1

x86avg = np.average(np.array(x86))
unvecavg = np.average(np.array(unvec))
ompavg = np.average(np.array(omp))

print("x86 ")
print(x86avg)
print("unvector ")
print(unvecavg)
print("omp ")
print(ompavg)
