#!/usr/bin/env python3

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

df = pd.read_csv("output.tsv", sep="\t")
a = df.values

X = a[:,0]
Y = a[:,1]
Z = a[:,2]

fig = plt.figure()
ax = Axes3D(fig)

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

ax.plot(X,Y,Z,marker="o")
#ax.plot(X,Y,Z,marker="o",linestyle='None')

plt.show()
