import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm

with open("../data/lab_1_1_data.txt", "r") as f:
    arr = np.loadtxt(f, dtype=float)

mean1_1_target = arr[0]
var1_1_target = arr[1]
num1_1_target = int(arr[2])
mean1_1_eval_mle = arr[-4]
var1_1_eval_mle = arr[-3]
mean1_1_eval_moe = arr[-2]
var1_1_eval_moe = arr[-1]
arr = arr[3:-4]

min1_1, max1_1 = np.min(arr), np.max(arr)
arr = np.sort(arr)

x_arr = np.zeros(num1_1_target)
y_arr = np.zeros(num1_1_target)
yac_arr = np.zeros(num1_1_target)
stride = (max1_1 - min1_1) / num1_1_target
for i in range(num1_1_target):
    x = min1_1 + i * stride
    y = len(np.where(arr < x)[0]) / num1_1_target
    yac = norm.cdf(x-mean1_1_target, scale=var1_1_target)
    x_arr[i], y_arr[i], yac_arr[i] = x, y, yac

plt.plot(x_arr, y_arr, "b-", label="Experimental Value")
plt.plot(x_arr, yac_arr, "r-", label="Theoretical Value")
plt.axis([min1_1-1, max1_1+1, 0, 1])
plt.xlabel("x", horizontalalignment="right")
plt.ylabel("y", rotation=0, verticalalignment="top")
plt.legend()
plt.show()