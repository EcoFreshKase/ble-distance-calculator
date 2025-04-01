import pandas as pd
import matplotlib.pyplot as plt
import os
import argparse
from math import sqrt
from typing import Dict, TypedDict
from matplotlib import colormaps

speedOfLight = 299792.458 # m/ms

class DeviatedResult(TypedDict):
    mean: float
    deviation: float
    df: pd.DataFrame
    durationData: pd.Series
    numOfBounceBack: int

class DeviatedDistance(TypedDict):
    distance: float
    deviation: float

def calcDeviation(csv_path: str, measurement_device_deviation: int, offset=0) -> DeviatedResult:
    df = pd.read_csv(csv_path)

    duration = df['end_time'] - df["start_time"] - offset

    linear_deviation = duration - duration.mean()
    quadratic_deviation = linear_deviation**2


    e_z = sum(quadratic_deviation / (len(duration) * (len(duration) - 1)))
    u = sqrt(measurement_device_deviation**2 + e_z)
    return {
        "mean": duration.mean(),
        "deviation": u,
        "df": df,
        "durationData": duration,
        "numOfBounceBack": df['num_of_passes'][0],
    }

def plotDataSet(data_set: pd.DataFrame, color="b", title='data'):
    plt.plot(data_set['id'], (data_set['end_time'] - data_set['start_time']) / 1000, marker='o', linestyle='None', color=color, label=title)

def calculateDistance(deviatedResult: DeviatedResult, numOfBounceBacks: int) -> DeviatedDistance:
    getDistance = lambda t: (t * speedOfLight) / (2 * numOfBounceBacks) # in m
    return {
        "distance": getDistance(deviatedResult["mean"]),
        "deviation": getDistance(deviatedResult["deviation"]),
    }

# measurement_device_deviation in ms and tcn_distance in mm
def calcTcn(csv_path: str, measurement_device_deviation: int, tcn_distance: int) -> DeviatedResult:
    df = pd.read_csv(csv_path)
    tcn = calcDeviation(csv_path, measurement_device_deviation, offset=2 * int(df['num_of_passes'][0]) * tcn_distance / speedOfLight)
    return tcn

if __name__ == "__main__":
    print()
    parser = argparse.ArgumentParser(description='Processes distance measurement data.')
    parser.add_argument('csv_path', type=str, nargs='+', help='Paths to the CSV files')
    parser.add_argument('--device_deviation', '-d', required=True, type=int, help='Measurement device deviation in ms')
    parser.add_argument('--plot', action='store_true', help='Plot the data')
    parser.add_argument('--tcn', type=str, nargs=2, metavar=('CSV_PATH', 'DISTANCE'), required=True,
                        help='TCN arguments. Takes the path to the csv file containing the measurement data to calculate tcn and an the distance of the tcn measurement in millimeters.')

    args = parser.parse_args()

    tcn_csv_path, tcn_distance = args.tcn
    measurement_device_deviation = args.device_deviation # ms

    tcn = calcTcn(tcn_csv_path, measurement_device_deviation, int(tcn_distance))
    print(f"TCN: {tcn['mean']} +/- {tcn['deviation']} ms")

    results = [(calcDeviation(csv_path, measurement_device_deviation, tcn['mean']), csv_path) for csv_path in args.csv_path]

    for res, csv_path in results:
        distance = calculateDistance(res, res['numOfBounceBack'])
        print(f"""{os.path.basename(csv_path)}:
              {res['mean']} +/- {res['deviation']} ms
              {distance['distance']} +/- {distance['deviation']} mm
              """)

    if args.plot:
        plt.figure(figsize=(10, 6))
        colorMap = colormaps['viridis']
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
# -d 1 --plot --tcn resources/experiment_5.csv 1000 resources/experiment_3.csv resources/experiment_5.csv
