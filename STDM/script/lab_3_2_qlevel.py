import matplotlib.pyplot as plt
from DP_utils import Data_Process_2

f = open("./jpeg_STDM_q_level.txt")
records = f.readlines()
records = [elem.strip('\n') for elem in records]

a = Data_Process_2()
a.process(records)

plt.plot(a.q_level_lst, a.pfa_lst)
plt.xlabel("q_level")
plt.ylabel("pfa")
plt.legend(["delta=4"])
plt.title("Pfa, when q_level in [1, 100]")
plt.show()