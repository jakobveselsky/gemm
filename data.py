import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

neon = pd.read_csv('neon.csv', sep=', ')
serial = pd.read_csv('serial.csv', sep=', ')
omp = pd.read_csv('omp3.csv', sep=', ')
pthreads = pd.read_csv('pthreads.csv', sep=', ')


print("serial")
print(serial)
print("neon")
print(neon)
print("omp")
print(omp)
print("pthreads")
print(pthreads)

plt.plot(neon["N"],neon["Time"], label='neon')
plt.plot(serial["N"],serial["Time"], label='serial')
plt.plot(omp["N"],omp["Time"], label='omp')
plt.plot(pthreads["N"],pthreads["Time"], label='omp')
plt.title('serial vs neon vs omp threads vs pthreads')
plt.xlabel('N')
plt.ylabel('Time')
plt.grid(True)
plt.legend()
plt.show()
