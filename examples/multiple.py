#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 2, 100)
x_2 = x**2
x_3 = x**3

fig, ax = plt.subplots(figsize=(6.4, 4.8))
fig.suptitle("multiple")
ax.plot(x, x, label='linear')
ax.plot(x, x_2, label='quadratic')
ax.plot(x, x_3, label='cubic')
ax.set_xlabel('x label')
ax.set_ylabel('y label')
ax.legend()
plt.show()