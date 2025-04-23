import pandas as pd
import matplotlib.pyplot as plt

data = [pd.read_csv('resources/data_burk_390.csv'), pd.read_csv('resources/data_burk_200.csv')]
data = [df[249:500] for df in data]

plt.figure(figsize=(15, 8))
plt.plot(data[0]['id'], data[0]['diffTime'], marker='o', linestyle='-', color='b', label='Burk 390')
plt.plot(data[1]['id'], data[1]['diffTime'], marker='o', linestyle='-', color='r', label='Burk 200')
plt.title('Difftime vs ID')
plt.xlabel('ID')
plt.ylabel('Difftime')
plt.legend()
plt.grid(True)
plt.show()
