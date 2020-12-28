import matplotlib.pyplot as plt
from DP_utils import Data_Process_1

f = open("../data/voice_STDM_delta.txt", "r")
records = f.readlines()
records = [elem.strip('\n') for elem in records]

a = Data_Process_1()
a.process(records)

plt.plot(a.delta_lst, a.pfa_lst)
plt.xlabel("delta")
plt.ylabel("pfa")
plt.legend(["mean=4, sigma=1"])
plt.title("Pfa, when delta in [0.1, 10]")
plt.show()