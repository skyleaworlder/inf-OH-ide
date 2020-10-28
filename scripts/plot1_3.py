import matplotlib.pyplot as plt
import numpy as np

with open("../data/lab_1_3_data1.txt", "r") as f:
    arr_c1 = np.loadtxt(f, dtype=float)
with open("../data/lab_1_3_data2.txt", "r") as f:
    arr_c05 = np.loadtxt(f, dtype=float)
with open("../data/lab_1_3_data3.txt", "r") as f:
    arr_c05t = np.loadtxt(f, dtype=float)

num1_3_target = int(arr_c1[1])
arr_c1 = arr_c1[2:]

min1_3_c1, max1_3_c1 = np.min(arr_c1), np.max(arr_c1)
x_c1arr = np.zeros(num1_3_target)
y_c1arr = np.zeros(num1_3_target)
stride = (max1_3_c1 - min1_3_c1) / num1_3_target
for i in range(num1_3_target):
    x = min1_3_c1 + i * stride
    y = len(np.where(arr_c1 < x)[0]) / num1_3_target
    x_c1arr[i], y_c1arr[i] = x, y

num1_3_target = int(arr_c05[1])
arr_c05 = arr_c05[2:]

min1_3_c05, max1_3_c05 = np.min(arr_c05), np.max(arr_c05)
x_c05arr = np.zeros(num1_3_target)
y_c05arr = np.zeros(num1_3_target)
stride = (max1_3_c05 - min1_3_c05) / num1_3_target
for i in range(num1_3_target):
    x = min1_3_c05 + i * stride
    y = len(np.where(arr_c05 < x)[0]) / num1_3_target
    x_c05arr[i], y_c05arr[i] = x, y

num1_3_target = int(arr_c05t[2])
x_c05tarr = np.array(arr_c05t[5:5+num1_3_target])
y_c05tarr = np.array(arr_c05t[5+num1_3_target:5+num1_3_target*2])
print(arr_c05t[5+num1_3_target:5+num1_3_target*2])

plt.plot(x_c1arr, y_c1arr, "b-", label="Experimental Value")
plt.plot(x_c05arr, y_c05arr, "r+", label="Experimental Value")
plt.plot(x_c05tarr, y_c05tarr, "g+", label="Theoretical Value")
plt.axis([min1_3_c1, max1_3_c1, 0, 1])
plt.xlabel("x", horizontalalignment="right")
plt.ylabel("y", rotation=0, verticalalignment="top")
plt.legend()
plt.show()