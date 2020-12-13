import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axisartist.axislines import SubplotZero
import matplotlib.ticker as ticker

pfa_theory = [1e-06, 1e-05, 0.0001, 0.001, 0.01, 0.1]
psi_theory = [1.50316, 1.34868, 1.17606, 0.977217, 0.735656, 0.405262]
pm_theory_a18 = [0.173949, 0.0767598, 0.024243, 0.00463588, 0.000381677, 5.15546e-06]
pm_theory_a15 = [0.503992, 0.316138, 0.152823, 0.0491467, 0.00782303, 0.000268223]

roc_c05_a18 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_a18,

    "pm": [0.1744, 0.07774, 0.02422, 0.00471, 0.00046, 1e-05],
    "pfa": [1e-6, 2e-05, 7e-05, 0.00117, 0.01036, 0.09913]
}

roc_c10_a18 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_a18,

    "pm": [0.17395, 0.07733, 0.02379, 0.00446, 0.00028, 1e-05],
    "pfa": [1e-6, 1e-5, 0.00012, 0.00085, 0.00994, 0.10018]
}

roc_c20_a18 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_a18,

    "pm": [0.17296, 0.07815, 0.02466, 0.00469, 0.00029, 1e-05],
    "pfa": [1e-6, 1e-5, 7e-05, 0.00094, 0.00991, 0.09975]
}

roc_c20_a15 = {
    "pfa_theory": pfa_theory,
    "psi_theory": psi_theory,
    "pm_theory": pm_theory_a15,

    "pm": [0.50287, 0.31624, 0.15618, 0.04894, 0.00815, 0.00017],
    "pfa": [1e-6, 2e-05, 0.00014, 0.00115, 0.01004, 0.09727]
}

fig = plt.figure(1, (10,6))
ax = SubplotZero(fig, 2,3,1)
bx = SubplotZero(fig, 2,3,2)
cx = SubplotZero(fig, 2,3,3)
dx = SubplotZero(fig, 2,3,4)
ex = SubplotZero(fig, 2,3,5)
fx = SubplotZero(fig, 2,3,6)
fig.add_subplot(ax)
fig.add_subplot(bx)
fig.add_subplot(cx)
fig.add_subplot(dx)
fig.add_subplot(ex)
fig.add_subplot(fx)

ax.set_ylim(1e-6, 1)
ax.set_xlim(1e-6, 1)
ax.set_yscale('log')
ax.set_xscale('log')
ax.set_title("c = 0.5 / a = 1.8")
ax.axis["bottom"].label.set_text("pm")
ax.axis["left"].label.set_text("pfa")
ax.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
ax.plot(roc_c05_a18["pm"], roc_c05_a18["pfa"], "b-", label="experimental")
ax.plot(roc_c05_a18["pm_theory"], roc_c05_a18["pfa_theory"], "g-", label="theoretical")


bx.set_ylim(1e-6, 1)
bx.set_xlim(1e-6, 1)
bx.set_yscale('log')
bx.set_xscale('log')
bx.set_title("c = 1.0 / a = 1.8")
bx.axis["bottom"].label.set_text("pm")
bx.axis["left"].label.set_text("pfa")
bx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
bx.plot(roc_c10_a18["pm"], roc_c10_a18["pfa"], "b-")
bx.plot(roc_c10_a18["pm_theory"], roc_c10_a18["pfa_theory"], "g-")


cx.set_ylim(1e-6, 1)
cx.set_xlim(1e-6, 1)
cx.set_yscale('log')
cx.set_xscale('log')
cx.set_title("c = 2.0 / a = 1.5")
cx.axis["bottom"].label.set_text("pm")
cx.axis["left"].label.set_text("pfa")
cx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
cx.plot(roc_c20_a15["pm"], roc_c20_a15["pfa"], "b-")
cx.plot(roc_c20_a15["pm_theory"], roc_c20_a15["pfa_theory"], "g-")


dx.set_ylim(1e-6, 1)
dx.set_xlim(1e-6, 1)
dx.set_yscale('log')
dx.set_xscale('log')
dx.set_title("c = 2.0 / a = 1.8")
dx.axis["bottom"].label.set_text("pm")
dx.axis["left"].label.set_text("pfa")
dx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
dx.plot(roc_c20_a18["pm"], roc_c20_a18["pfa"], "b-")
dx.plot(roc_c20_a18["pm_theory"], roc_c20_a18["pfa_theory"], "g-")

ex.set_ylim(1e-6, 1)
ex.set_xlim(1e-6, 1)
ex.set_yscale('log')
ex.set_xscale('log')
ex.set_title("c = 0.5 / c = 1.0 / c = 2.0 | a = 1.8")
ex.axis["bottom"].label.set_text("pm")
ex.axis["left"].label.set_text("pfa")
ex.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
ex.plot(roc_c05_a18["pm"], roc_c05_a18["pfa"], "r-", label="experimental")
ex.plot(roc_c10_a18["pm"], roc_c10_a18["pfa"], "g-", label="experimental")
ex.plot(roc_c20_a18["pm"], roc_c20_a18["pfa"], "y-", label="experimental")

fx.set_ylim(1e-6, 1)
fx.set_xlim(1e-6, 1)
fx.set_yscale('log')
fx.set_xscale('log')
fx.set_title("c = 2.0 | a = 1.8 / a = 1.5")
fx.axis["bottom"].label.set_text("pm")
fx.axis["left"].label.set_text("pfa")
fx.yaxis.set_major_locator(ticker.LogLocator(base=100.0, numticks=5))
fx.plot(roc_c20_a15["pm"], roc_c20_a15["pfa"], "r-", label="experimental")
fx.plot(roc_c20_a18["pm"], roc_c20_a18["pfa"], "g-", label="experimental")

plt.show()