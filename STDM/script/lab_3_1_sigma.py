import matplotlib.pyplot as plt
from DP_utils import Data_Process_1

f = open("./voice_STDM_sigma.txt")
records = f.readlines()
records = [elem.strip('\n') for elem in records]

a = Data_Process_1()
a.process(records)

plt.plot(a.sigma_lst, a.pfa_lst)
plt.xlabel("sigma")
plt.ylabel("pfa")
plt.legend(["delta=4, mean=1"])
plt.title("Pfa, when sigma in [0.1, 13.9]")
plt.show()