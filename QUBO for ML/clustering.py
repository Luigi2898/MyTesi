# %%
from functools import partial
from math import floor

import matplotlib.pyplot as plt
import numpy as np
from dwave import inspector
from dwave.embedding.chain_strength import uniform_torque_compensation
from dwave.system import EmbeddingComposite, DWaveSampler
from numpy import array
from qubovert.utils import matrix_to_qubo
from sklearn.datasets import make_blobs
from sklearn.preprocessing import StandardScaler

# ! Tunable parameters
N = 6
k = 2
# * Synthetic dataset definition
features, true_labels = make_blobs(n_samples=N, centers=k, cluster_std=1.00, random_state=42)
scaler = StandardScaler()
scaled_features = scaler.fit_transform(features)
x = array(scaled_features)
# * Dataset visualization (uncomment to plot)
plt.figure()
plt.plot(x[:, 0], x[:, 1], 'o')
plt.show()
# * Identity matrices and ones matrices
I_K = np.identity(k)
I_N = np.identity(N)
ONE_N = np.ones([N, N])
ONE_K = np.ones([k, k])
# * Matrix D
D = np.zeros([N, N])
for i in range(N):
	for j in range(N):
		D[i, j] = sum(x[i, :] - x[j, :]) ** 2
# * Definition of alpha and beta
alpha = 10
beta = 50
# * F matrix
F = ONE_N - (2 * N / k) * I_N
# * G matrix
G = ONE_K - 2 * I_K
# * Q matrix
Q = np.zeros([N * k, N * k])
for i in range(N * k):
	for j in range(N * k):
		Q[i, j] = 1 if j == (N * ((i - 1) % k) + floor((i - 1) / k) + 1) else 0
# * Definition of the model
qubo = np.kron(I_K, (D + alpha * F)) + np.dot(np.dot(Q.T, np.kron(I_N, beta * G)), Q)

Qubo = matrix_to_qubo(qubo)
# * DWave sampling
dwave_qubo = Qubo.Q
print(f"Start embedding. Problem has {Qubo.num_binary_variables} binary variables.")
chain_strength = partial(uniform_torque_compensation, prefactor=5)
sampler = EmbeddingComposite(DWaveSampler())
# sampler =  LeapHybridSampler()
sampleset = sampler.sample_qubo(dwave_qubo, num_reads=100, chain_strength=chain_strength)
# sampleset = sampler.sample_qubo(dwave_qubo)
qubo_solution = sampleset.first.sample
inspector.show(sampleset)
# * Solution display
print("Variable assignment :", qubo_solution)
print("Model value:", Qubo.value(qubo_solution))
for j in range(k):
	for i in range(N):
		print(qubo_solution[i + j], end=' ')
	print("\n")
# * Solution interpretation
foundLables = np.zeros([N, 1])
for i in range(0, N):
	for j in range(k):
		if qubo_solution[i + j] == 1:
			foundLables[i] = j
# * Error counting           
err = 0
for i in range(N):
	if foundLables[i] != true_labels[i]:
		err += 1
print(err)
# * Result visualization (uncomment to plot)
plt.figure()
x0 = x[np.where(foundLables == 0), :]
plt.plot(x0[:, :, 0], x0[:, :, 1], 'or')
x1 = x[np.where(foundLables == 1), :]
plt.plot(x1[:, :, 0], x1[:, :, 1], 'ob')
x2 = x[np.where(foundLables == 2), :]
plt.plot(x2[:, :, 0], x2[:, :, 1], 'og')
x3 = x[np.where(foundLables == 3), :]
plt.plot(x3[:, :, 0], x3[:, :, 1], 'oc')
plt.show()
