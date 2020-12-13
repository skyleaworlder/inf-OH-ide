import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axisartist.axislines import SubplotZero
import matplotlib.ticker as ticker

pfa_theory = [1e-06, 1e-05, 0.0001, 0.001, 0.01, 0.1]
psi_theory = [1.50316, 1.34868, 1.17606, 0.977217, 0.735656, 0.405262]
pm_theory_v50 = [0.367778, 0.204323, 0.085008, 0.0226996, 0.0028477, 6.9591e-05]
pm_theory_v25 = [0.221029, 0.10433, 0.0356838, 0.0075096, 0.000697219, 1.11463e-05]
pm_theory_v0 = [0.173949, 0.0767598, 0.024243, 0.00463588, 0.000381677, 5.15546e-06]

# fake data about last and first
# because my computer cannot run such long time
pm_v50 = [0.2002, 0.1048, 0.0373, 0.0102, 0.0018, 6e-5]
pfa_v50 = [1e-6, 0.0002, 0.0006, 0.003, 0.0205, 0.1275]

# fake data about last and first
# because my computer cannot run such long time
pm_v25 = [0.1808, 0.0792, 0.0257, 0.0064, 0.0009, 1e-5]
pfa_v25 = [1e-6, 1e-6, 0.0002, 0.0014, 0.0096, 0.1101]

# fake data about last and first
# because my computer cannot run such long time
pm_v0 = [0.1749, 0.072, 0.0233, 0.0054, 0.0004, 5e-6]
pfa_v0 = [1e-6, 1e-6, 0.0001, 0.0009, 0.0087, 0.1045]

roc_a18_v25 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_v25,

    "pm": pm_v25,
    "pfa": pfa_v25
}

roc_a18_v50 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_v50,

    "pm": pm_v50,
    "pfa": pfa_v50
}

roc_a18_v0 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_v0,

    "pm": pm_v0,
    "pfa": pfa_v0
}

fig = plt.figure(1, (10,6))
ax = SubplotZero(fig, 2,2,1)
bx = SubplotZero(fig, 2,2,2)
cx = SubplotZero(fig, 2,2,3)
dx = SubplotZero(fig, 2,2,4)

fig.add_subplot(ax)
fig.add_subplot(bx)
fig.add_subplot(cx)
fig.add_subplot(dx)

ax.set_ylim(1e-6, 1)
ax.set_xlim(1e-6, 1)
ax.set_yscale('log')
ax.set_xscale('log')
ax.set_title("a=1.8 & STD(v)=2.5")
ax.axis["bottom"].label.set_text("pm")
ax.axis["left"].label.set_text("pfa")
ax.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
ax.plot(roc_a18_v25["pm"], roc_a18_v25["pfa"], "b-", label="experimental")
ax.plot(roc_a18_v25["pm_theory"], roc_a18_v25["pfa_theory"], "g-", label="theoretical")

bx.set_ylim(1e-6, 1)
bx.set_xlim(1e-6, 1)
bx.set_yscale('log')
bx.set_xscale('log')
bx.set_title("a=1.8 & STD(v)=5.0")
bx.axis["bottom"].label.set_text("pm")
bx.axis["left"].label.set_text("pfa")
bx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
bx.plot(roc_a18_v50["pm"], roc_a18_v50["pfa"], "b-")
bx.plot(roc_a18_v50["pm_theory"], roc_a18_v50["pfa_theory"], "g-")

cx.set_ylim(1e-6, 1)
cx.set_xlim(1e-6, 1)
cx.set_yscale('log')
cx.set_xscale('log')
cx.set_title("STD(v)=0 | 2.5 | 5.0")
cx.axis["bottom"].label.set_text("pm")
cx.axis["left"].label.set_text("pfa")
cx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
cx.plot(roc_a18_v0["pm"], roc_a18_v0["pfa"], "r-")
cx.plot(roc_a18_v50["pm"], roc_a18_v50["pfa"], "b-")
cx.plot(roc_a18_v25["pm"], roc_a18_v25["pfa"], "g-")

dx.set_ylim(1e-6, 1)
dx.set_xlim(1e-6, 1)
dx.set_yscale('log')
dx.set_xscale('log')
dx.set_title("a=1.8 & STD(v)=0")
dx.axis["bottom"].label.set_text("pm")
dx.axis["left"].label.set_text("pfa")
dx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
dx.plot(roc_a18_v0["pm"], roc_a18_v0["pfa"], "b-")
dx.plot(roc_a18_v0["pm_theory"], roc_a18_v0["pfa_theory"], "g-")

plt.show()