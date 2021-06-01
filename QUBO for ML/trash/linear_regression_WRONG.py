import sys

import numpy as np
from qubovert import spin_var, sim, PUSO, PUBO, boolean_var, QUBO
from qubovert.utils import qubo_to_matrix, spin_to_boolean
from utils import produce_dir, ConvertSol, MSE, objectHandler

np.set_printoptions(linewidth=1000)

n = int(sys.argv[1])          # Number of points in dataset
d = int(sys.argv[2])          # Number of features of dataset
slope = float(sys.argv[3])      # Slope of the dataset
intercept = float(sys.argv[4])  # Intercept of the dataset
noise = float(sys.argv[5])      # Noise amplitude
nPow = int(sys.argv[6])       # Number of power of two
AD = int(sys.argv[7])
NA = int(sys.argv[8])

# print("-- Dataset definition --")
# Definition of dataset
r = np.random.RandomState()
y = []
x = np.zeros((n, 2))
x1 = np.linspace(-10, 10, n)
for i in range(n):
	e = r.uniform(-noise, noise)
	y.append(slope * x1[i] + intercept + e)  # Definition of Y
	x[i] = np.hstack([x1[i], 1])             # Definition of X (adding the ones)
y = np.array(y)

# Dataset figure saving
# fig = ex.scatter(x=x[:, 0], y=y)
# p = f"dataset__{n}__{d}__{slope}__{intercept}"
# p = p.replace('.', '_')
# fig.write_html(f"./{p}.html")

# Model definition
# print("-- Model definition --")
twop = np.linspace(-nPow + 1, 0, nPow)                     # Definition of the power of precision vector
nPow = nPow * 2                                              # There will be a specular series of powers
P = np.hstack((-(2 ** twop), 2 ** twop))   
P = np.sort(P)                                               # Precision vector sorting
Nw = nPow * (d + 1)                                          # Number of binary variables computation
Pc = np.kron(np.identity(d + 1), P.T)                        # Precision array definition
A1 = np.dot(Pc.T, x.T)
A2 = np.dot(A1, x)
A = np.dot(A2, Pc)								 # A matrix definition
b1 = np.dot(Pc.T, x.T)
b = -2 * np.dot(b1, y)                       # b vector definition
wHat = {i: boolean_var(i) for i in range(Nw)}     # Variables definition
# Model definition
model = QUBO()
for j in range(Nw):
	for i in range(j):
		model += A[i, j] * wHat[i] * wHat[j]
for i in range(Nw):
	model += b[i] * wHat[i]
qubo = model                                       # PUBO > QUBO
qubo.normalize()


# print("-- Annealing --")
# Annealing and ground-state solution selection
#initialState = {i: boolean_var('w_hat(%d)' % i) for i in range(Nw)}
initialState = {}
for i in range(Nw):
	initialState[i] = 0
print(initialState)
oldV = 0
for _ in range(NA):
	anneal_res = sim.anneal_qubo(qubo, anneal_duration=AD, initial_state=initialState)
	qubo_solution = anneal_res.best.state
	V = anneal_res.best.value
	if (V <= oldV):
		initialState = qubo_solution
		oldV = V
print(qubo_solution)
qubo_solution1 = model.convert_solution(qubo_solution) # Inverte la soluzione!!!
print(qubo_solution1)
# Dictionary > vector


# print("-- Solution interpretation --")
wHatSol = np.array(ConvertSol(qubo_solution1))
print(wHatSol)

wSol = np.dot(Pc, wHatSol)  # Coefficient calculation
yFound = np.dot(x, wSol)  # Resulting y calculation


#selctedPrec = DF([[P[i] if wHatSol[i] == 1 else 0 for i in range(nPow)], [P[i] if wHatSol[nPow + i] == 1 else 0 for i in range(nPow)]], ['w[0]', 'w[1]'])

# fig = Figure()
# fig.add_trace(Scatter(x=x[:, 0], y=y.T, mode='markers', name='Original'))
# fig.add_trace(Scatter(x=x[:, 0], y=yFound, mode='lines', name='Simulated annealing'))
# p = f"result__{n}__{d}__{slope}__{intercept}__{nPow}"
# p = p.replace('.', '_')
# fig.write_html(f"./{p}.html")

p = f"report__{n}__{d}__{slope}__{intercept}__{nPow}"
p = p.replace('.', '_')
with open(f"./{p}.rpt", "w") as of:
	of.write("-- LINEAR REGRESSION REPORT --\n")
	of.write("Problem parameters:\n")
	of.write(f"\t- Number of elements in dataset: {n}\n")
	of.write(f"\t- A: {slope}\n")
	of.write(f"\t- B: {intercept}\n")
	of.write(f"\t- e ~ U({-noise}, {noise})\n")
	of.write(f"\t- y = A * x1 + B + e\n")
	of.write(f"\t- Dimensionality of dataset: {d}\n")
	of.write(f"\t- Number of elements in precision array: {nPow}\n")
	of.write(f"\t- QUBO terms: {qubo.num_terms}\n")
	of.write(f"\t- Number of binary variables: {Nw}\n\n\n\n")
	of.write(f"\t- Precision powers:\t{twop}\n")
	of.write(f"\t- Precision array:\t{P}\n\n\n")
	of.write("Solution parameters\n")
	of.write(f"\t- Solution is: \n{qubo_solution}\n")
	of.write(f"\t- Selected weights for w[0] are: \n{[P[i] if wHatSol[i] == 1 else 0 for i in range(nPow)]}\n")
	of.write(f"\t- Selected weights for w[1] are: \n{[P[i] if wHatSol[nPow + i] == 1 else 0 for i in range(nPow)]}\n")
	of.write(f"\t- Linear regression solution: {wSol}\n")
	of.write(f"\t- MSE: {MSE(y, yFound, n)}\n")
	of.write("Solver parameters:\n")
	of.write(f"\t- Number of annealing: {NA}\n")
	of.write(f"\t- Duration of annealing: {AD}\n")

p = f"points__{n}__{d}__{slope}__{intercept}__{nPow}"
p = p.replace('.', '_')
with open(f"./{p}.pnt", "w") as of:
	for i, _ in enumerate(x):
		of.write(f"{x[i, 0]},{y[i]},{yFound[i]}\n")

p = f"qubo__{n}__{d}__{slope}__{intercept}__{nPow}"
p = p.replace('.', '_')
with open(f"./{p}.qubo", "w") as of:
	of.write("QUBO matrix:\n\n")
	of.write(str(qubo.Q))
