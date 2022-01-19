#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

delta = 0.025
x = np.arange(-3.0, 3.0001, delta)
y = np.arange(-2.0, 2.0001, delta)
X, Y = np.meshgrid(x, y)
Z1 = np.exp(-X**2 - Y**2)
Z2 = np.exp(-(X - 1)**2 - (Y - 1)**2)
Z = (Z1 - Z2) * 2

fig, ax = plt.subplots()
CS = ax.contourf(X, Y, Z)
cbar = fig.colorbar(CS)
cbar.ax.set_ylabel("Height")
ax.set_xlabel("$x$")
ax.set_ylabel("$y$")
ax.set_title("Shaded contours")

plt.show()