import pandas as pd
import matplotlib.pyplot as plt
import os
import argparse
from math import sqrt
from typing import Dict
from matplotlib import cm

def calcDeviation(csv_path: str, measurement_device_deviation: int) -> Dict[str, float]:
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
    parser = argparse.ArgumentParser(description='Processes distance measurement data.')
    parser.add_argument('csv_path', type=str, nargs='+', help='Paths to the CSV files')
    parser.add_argument('--device_deviation', '-d', required=True, type=int, help='Measurement device deviation in ms')
    parser.add_argument('--plot', action='store_true', help='Plot the data')

    args = parser.parse_args()
    measurement_device_deviation = args.device_deviation # ms

    results = [(calcDeviation(csv_path, measurement_device_deviation), csv_path) for csv_path in args.csv_path]

    for res, csv_path in results:
        print(f"{os.path.basename(csv_path)}: {res['mean']} +/- {res['deviation']} ms")

    if args.plot:
        plt.figure(figsize=(10, 6))
        colorMap = cm.get_cmap('viridis', len(results))
        for i, (res, csv_path) in enumerate(results):
            color = colorMap(i / len(results))
            plotDataSet(res["df"], color=color, title=os.path.basename(csv_path))
        plt.xlabel('ID')
        plt.ylabel('Duration in s')
        plt.title('Duration in s over time')
        plt.legend()
        plt.grid(True)
        plt.show()

# cur command:
# -d 1 --plot resources/experiment_2.csv resources/experiment_3.csv resources/experiment_4.csv
