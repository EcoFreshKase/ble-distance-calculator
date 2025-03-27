import pandas as pd
import os
from math import sqrt

csv_path = os.path.join("resources", "example.csv")
measurement_device_deviation = 1 # ms

df = pd.read_csv(csv_path)

duration = df['end_time'] - df["start_time"]

linear_deviation = duration - duration.mean()
quadratic_deviation = linear_deviation**2


e_z = sum(quadratic_deviation / (len(duration) * (len(duration) - 1)))
u = sqrt(measurement_device_deviation**2 + e_z)

print(f"Measurement result: {duration.mean()} +/- {u} ms")
