import os
from datetime import datetime

import matplotlib.pyplot as plt
import numpy as np
from numpy import array
from qubovert import boolean_var
from qubovert.sim import anneal_qubo, anneal_temperature_range
from sklearn.datasets import make_blobs
from sklearn.preprocessing import StandardScaler

from utils import distance_matrix, to_matlab

N = 10
k = 2
lamRow = 100  # one hot constraint
lamCol = 500  # Equal size clusters
elmPerK = N / k
NAnn = 1000
DAnn = 1000000
SProb = 0.9
EProb = 0.1

features, true_labels = make_blobs(n_samples=N, centers=k, cluster_std=0.50, random_state=42)
scaler = StandardScaler()
scaled_features = scaler.fit_transform(features)
x = array(scaled_features)

plt.figure()
plt.plot(x[:, 0], x[:, 1], 'o')
# plt.show(block=False)
# ? Result directory construction
parentdir = f"../Results/Cluster/classic/"
dir = datetime.now()
dir = dir.strftime("%m%d%Y_%H_%M_%S")
path = os.path.join(parentdir, dir)
while True:
	try:
		os.mkdir(path)
	except FileExistsError:
		print("Folder is already present, directory will be renamed.")
		path = path + "__new"
		continue
	else:
		break
print(f"Results are in {path}")
print("-" * 80)
Nw = N * k
I_k = np.identity(k)

D = distance_matrix(x)
wHat = {i: boolean_var('w_hat(%d)' % i) for i in range(Nw)}
A = np.kron(I_k, D)

model = 0
for j in range(Nw):
	for i in range(j):
		model += A[i, j] * wHat[i] * wHat[j]

for i in range(N):
	rSum = 0
	for j in range(k):
		rSum += wHat[j + k * i]
	model.add_constraint_eq_zero(rSum - 1, lam=lamRow)

for i in range(k):
	cSum = 0
	for j in range(N):
		cSum += wHat[j + k * i]
	model.add_constraint_eq_zero(cSum - elmPerK, lam=lamCol)

qubo = model.to_qubo()
qubo.normalize()
print(f"Start solving. Problem has {qubo.num_binary_variables} binary variables.")
anneal_res = anneal_qubo(qubo, num_anneals=NAnn, in_order=True, anneal_duration=DAnn,
						 temperature_range=anneal_temperature_range(qubo, start_flip_prob=SProb, end_flip_prob=EProb,
																	spin=False), schedule='linear')
qubo_solution = anneal_res.best.state

# print(qubo_solution)

s = []
for el in qubo_solution:
	s.append(qubo_solution[el])
sol = []
for i in range(N):
	r = []
	for j in range(k):
		r.append(s[j + k * i])
	sol.append(r)

code = np.zeros((k, k))
i = k - 1
for r in range(k):
	code[r][i] = 1
	i -= 1
code = tuple(map(tuple, code))

# print(code)

Flab = np.zeros((N, 1))
for i, row in enumerate(sol):
	if tuple(row) in code:
		Flab[i] = code.index(tuple(row))
	else:
		Flab[i] = -1

# print(Flab)

plt.figure()
for lab in range(k):
	i = np.where(Flab == lab)[0]
	print(i)
	plt.plot(x[i, 0], x[i, 1], 'o')

i = np.where(Flab == -1)[0]
plt.plot(x[i, 0], x[i, 1], '*')
plt.savefig(path + "/" + "visualization.png")
# plt.show(block=False)

modelS = qubo.pretty_str()
modelM = to_matlab(modelS)
with open(path + "/" + "report.txt", 'w') as of:
	of.write("-- Clustering by simulated annealing report --\n\n\n\n")
	of.write("Problem parameters\n")
	of.write(f"\t- Number of elements in dataset: {N}\n")
	of.write(f"\t- Number of clusters: {k}\n")
	of.write(f"\t- Number of elements per cluster: {elmPerK}\n")
	of.write(f"\t- One hot constraint weight: {lamRow}\n")
	of.write(f"\t- Equally sized clusters weight: {lamCol}\n")
	of.write(f"\t- Number of binary variabes: {Nw}\n\n\n\n")
	of.write(f"Model is:\t{modelS}\n\n\n")
	of.write(f"Model is (MATLAB):\t{modelM}\n\n\n")
	of.write("Solver parameters\n")
	of.write(f"\t- Number of annealing is: {NAnn}\n")
	of.write(f"\t- Annealing duration is: {DAnn}\n")
	of.write(f"\t- Start and stop probabilities are: {SProb} -> {EProb}\n\n\n\n")
	of.write("Solution parameters\n")
	of.write(f"\t- Solution is: {qubo_solution}\n")
	of.write(f"\t- Model value is: {qubo.value(qubo_solution)}\n")
	of.write(f"\t- Pretty solution is: {sol}\n")
	of.write(f"\t- Found labels are: {Flab}\n")
os.system(f'caja {path}')

input("Press enter to stop")
