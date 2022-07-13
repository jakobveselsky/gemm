import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

neon = pd.read_csv('neon.csv', sep=', ')
serial = pd.read_csv('serial.csv', sep=', ')

print("serial")
print(serial)
print("neon")
print(neon)

plt.plot(neon["N"],neon["time"], label='neon')
plt.plot(serial["N"],serial["time"], label='serial')
plt.title('serial vs neon')
plt.xlabel('N')
plt.ylabel('Time')
plt.grid(True)
plt.legend()
plt.show()
