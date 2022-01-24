#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 2, 100)
x_2 = x**2
x_3 = x**3
x_4 = x**4

fig, ax = plt.subplots(2, 2)
row0, row1 = ax
ax00, ax01 = row0
ax10, ax11 = row1

fig.suptitle("subplots")
ax00.plot(x, x)
ax00.text(0.05, 0.95, '$x$', ha='left', va='top', transform=ax00.transAxes)
ax01.plot(x, x_2)
ax01.text(0.05, 0.95, '$x^2$', ha='left', va='top', transform=ax01.transAxes)
ax10.plot(x, x_3)
ax10.text(0.05, 0.95, '$x^3$', ha='left', va='top', transform=ax10.transAxes)
ax11.plot(x, x_4)
ax11.text(0.05, 0.95, '$x^4$', ha='left', va='top', transform=ax11.transAxes)

ax10.set_xlabel('$x$')
ax11.set_xlabel('$x$')

ax00.set_ylabel('$f(x)$')
ax10.set_ylabel('$f(x)$')
plt.show()