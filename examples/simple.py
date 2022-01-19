#!/usr/bin/python3

import matplotlib.pyplot as plt

x = [0, 1, 2, 3]
y = [0, 1, 4, 9]

fig, ax = plt.subplots()
ax.plot(x, y, "r")
plt.show()