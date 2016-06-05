import numpy as np
import math


cells = list(np.zeros(101))

cells[0] = 42000
cells[100] = 420

gens = [list(cells)]
alpha = 0.1

for i in range(0, 100):
    cells2 = list(cells)
    for j in range(1,100):
        cells[j] = min(math.floor((cells2[j-1] + cells2[j] + cells2[j+1]) * alpha), 65535)

    gens.append(list(cells))


with open('A33.csv', 'w') as output:
    for gen in gens:
        for item in gen:
            output.write("%s " % item)
        output.write("\n")
