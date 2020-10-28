import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import expon

with open("../data/lab_1_2_data.txt", "r") as f:
    arr = np.loadtxt(f, dtype=float)

''' it has to be noticed that beta is scale parameter,
    while sigma is scale parameter in normal distribution.
'''
beta1_2_target = arr[0]
num1_2_target = int(arr[1])
arr = arr[2:-1]

min1_2, max1_2 = np.min(arr), np.max(arr)
mean1_2_target = np.mean(arr)
arr = np.sort(arr)

x_arr = np.zeros(num1_2_target)
y_arr = np.zeros(num1_2_target)
yac_arr = np.zeros(num1_2_target)
stride = (max1_2 - min1_2) / num1_2_target
for i in range(num1_2_target):
    x = min1_2 + i * stride
    y = len(np.where(arr < x)[0]) / num1_2_target
    yac = expon.cdf(x, scale=beta1_2_target)
    x_arr[i], y_arr[i], yac_arr[i] = x, y, yac

plt.plot(x_arr, y_arr, "b-", label="Experimental Value")
plt.plot(x_arr, yac_arr, "r-", label="Theoretical Value")
plt.axis([min1_2-1, max1_2+1, 0, 1])
plt.xlabel("x", horizontalalignment="right")
plt.ylabel("y", rotation=0, verticalalignment="top")
plt.legend()
plt.show()