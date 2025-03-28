import pandas as pd
import matplotlib.pyplot as plt
import os
from math import sqrt
from typing import Dict

csv_path_1 = os.path.join("resources", "experiment_2.csv")
csv_path_2 = os.path.join("resources", "experiment_3.csv")
measurement_device_deviation = 1 # ms

def calcDeviation(csv_path) -> Dict[str, float]:
    df = pd.read_csv(csv_path)

    duration = df['end_time'] - df["start_time"]

    linear_deviation = duration - duration.mean()
    quadratic_deviation = linear_deviation**2


    e_z = sum(quadratic_deviation / (len(duration) * (len(duration) - 1)))
    u = sqrt(measurement_device_deviation**2 + e_z)
    return {
        "mean": duration.mean(),
        "deviation": u,
        "df": df,
        "durationData": duration
    }

res1 = calcDeviation(csv_path_1)
res2 = calcDeviation(csv_path_2)

print(f"Measurement result 1: {res1["mean"]} +/- {res1["deviation"]} ms")
print(f"Measurement result 2: {res2["mean"]} +/- {res2["deviation"]} ms")

plt.figure(figsize=(10, 6))
plt.plot(res1["df"]['id'], res1["durationData"] / 1000, marker='o', linestyle='None', color='b', label='res1 Data')
plt.plot(res2["df"]['id'], res2["durationData"] / 1000, marker='o', linestyle='None', color='r', label='res2 Data')
plt.xlabel('ID')
plt.ylabel('Duration in s')
plt.title('Duration in s over time')
plt.legend()
plt.grid(True)
plt.show()