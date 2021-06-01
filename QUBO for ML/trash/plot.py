from matplotlib import pyplot as plt

xV = []
yV = []
yFV = []

with open("../output/Regressione lineare/sim/1000P/1000Annealing/1000Duration/15Precision/points__1000__1__0_8__0_75__30.pnt", 'r') as inF:
    lines = inF.readlines()
    for pnt in lines:
        [x, y, yF] = pnt.split(',')
        xV.append(float(x))
        yV.append(float(y))
        yFV.append(float(yF))

plt.figure()
plt.plot(xV, yV)
plt.plot(xV, yFV)
plt.show()