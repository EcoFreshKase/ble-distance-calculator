import pandas as pd
import matplotlib.pyplot as plt
import os
from math import sqrt
from typing import Dict

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

def plotDataSet(data_set: pd.DataFrame, color="b", title='data'):
    plt.plot(data_set['id'], (data_set['end_time'] - data_set['start_time']) / 1000, marker='o', linestyle='None', color=color, label=title)

if __name__ == "__main__":
    csv_path_1 = os.path.join("resources", "experiment_2.csv")
    csv_path_2 = os.path.join("resources", "experiment_3.csv")
    csv_path_3 = os.path.join("resources", "experiment_4.csv")
    measurement_device_deviation = 1 # ms

    res1 = calcDeviation(csv_path_1)
    res2 = calcDeviation(csv_path_2)
    res3 = calcDeviation(csv_path_3)

    print(f"Measurement result 1: {res1['mean']} +/- {res1['deviation']} ms")
    print(f"Measurement result 2: {res2['mean']} +/- {res2['deviation']} ms")
    print(f"Measurement result 3: {res3['mean']} +/- {res3['deviation']} ms")

    plt.figure(figsize=(10, 6))
    plotDataSet(res1["df"], color='b', title='res1')
    plotDataSet(res2["df"], color='r', title='res2')
    plotDataSet(res3["df"], color='y', title='res3')
    plt.xlabel('ID')
    plt.ylabel('Duration in s')
    plt.title('Duration in s over time')
    plt.legend()
    plt.grid(True)
    plt.show()
