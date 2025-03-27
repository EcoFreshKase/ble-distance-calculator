import pandas as pd
import matplotlib.pyplot as plt
import os
from math import sqrt

csv_path = os.path.join("resources", "experiment_2.csv")
measurement_device_deviation = 1 # ms

df = pd.read_csv(csv_path)

duration = df['end_time'] - df["start_time"]

linear_deviation = duration - duration.mean()
quadratic_deviation = linear_deviation**2


e_z = sum(quadratic_deviation / (len(duration) * (len(duration) - 1)))
u = sqrt(measurement_device_deviation**2 + e_z)

print(f"Measurement result: {duration.mean()} +/- {u} ms")

plt.figure(figsize=(10, 6))
plt.plot(df['id'], duration / 1000, marker='o', linestyle='None', color='b', label='Duration')
plt.xlabel('ID')
plt.ylabel('Duration in s')
plt.title('Duration in s over time')
plt.legend()
plt.grid(True)
plt.show()