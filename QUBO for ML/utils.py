import os
import re
from datetime import datetime
from functools import partial

import neal
import numpy as np
from dwave import inspector
from dwave.embedding.chain_strength import uniform_torque_compensation
from dwave.system import EmbeddingComposite, DWaveSampler
from qubovert import sim
from qubovert import spin_var
from sklearn.datasets import make_blobs
from sklearn.preprocessing import StandardScaler


def distance_matrix(x):
	d = np.zeros((len(x), len(x)))
	for i in range(len(x)):
		for j in range(len(x)):
			d[i, j] = (x[i, 0] - x[j, 0]) ** 2 + (x[i, 1] - x[j, 1]) ** 2
	return d


def to_matlab(pretty_string):
	matlab = pretty_string
	matlab = matlab.replace(') x(', ') * x(')
	matlab = re.sub(r'(\d)([ x])', r'\1 *\2', matlab)
	matlab = re.sub(r'([x(])(\d|\dd)([)])', r'\1\2 + 1\3', matlab)
	return matlab


def produce_dir(parentdir):
	name = datetime.now()
	name = name.strftime("%m%d%Y_%H_%M_%S")
	path = os.path.join(parentdir, name)
	while True:
		try:
			os.mkdir(path)
		except FileExistsError:
			print("Folder is already present, directory will be renamed.")
			path = path + "__new"
			continue
		else:
			break
	return path, name


def data_set_clu(n, k):
	features, true_labels = make_blobs(n_samples=n, centers=k, cluster_std=0.50, random_state=42)
	scaler = StandardScaler()
	scaled_features = scaler.fit_transform(features)
	x = np.array(scaled_features)
	return x


def data_set(n):
	R = np.random.RandomState()
	y = []
	x = np.zeros((n, 2))
	AA = R.uniform(0.5, 1)
	B = R.uniform(0.5, 1)
	for i in range(n):
		x1 = R.uniform(0.1, 5)
		e = R.uniform(0.1, 0.5)
		y.append(AA * x1 + B + e)
		x[i] = np.hstack([x1, 1])
	y = np.array(y).T
	N = len(x)
	d = len(x[0, :]) - 1
	return x, y, N, d, AA, B


def model_def_clu(x, n, k, lam_row, lam_col):
	nw = n * k
	I_k = np.identity(k)
	dist = distance_matrix(x)
	w_hat = {i: spin_var('w_hat(%d)' % i) for i in range(nw)}
	a = np.kron(I_k, dist)
	model = 0
	for j in range(nw):
		for i in range(j):
			model += a[j, i] * w_hat[i] * w_hat[j]
	for i in range(n):
		r_sum = 0
		for j in range(k):
			r_sum += w_hat[j + k * i]
		model.add_constraint_eq_zero(r_sum - 1, lam=lam_row)
	for i in range(k):
		c_sum = 0
		for j in range(n):
			c_sum += w_hat[j + k * i]
		model.add_constraint_eq_zero(c_sum - int(n/k), lam=lam_col)
	qubo = model.to_qubo()
	qubo.normalize()
	return qubo


def model_def(Pstart, Pstop, K, x, y, d):
	twop = np.linspace(Pstart, Pstop, K).T
	K = K * 2
	P = np.hstack((-(2 ** twop), 2 ** twop))
	P = np.sort(P)
	Nw = K * (d + 1)
	wHat = {i: spin_var('w_hat(%d)' % i) for i in range(Nw)}
	Pc = np.kron(np.identity(d + 1), P.T)
	A = np.dot(Pc.T, np.dot(np.dot(x.T, x), Pc))
	b = - 2 * np.dot(Pc.T, np.dot(x.T, y))
	model = 0
	for i in range(Nw):
		model += b[i] * wHat[i]
	for j in range(Nw):
		for i in range(j):
			model += A[j, i] * wHat[i] * wHat[j]
	qubo = model.to_qubo()
	qubo.normalize()
	return (Nw, qubo, twop, P, Pc)


def qubov_anneal(qubo):
	anneal_res = sim.anneal_qubo(qubo)
	qubo_solution = anneal_res.best.state
	return qubo_solution


def ConvertSol(qubo_solution):
	#sorted_sol = sorted(qubo_solution)
	#print(sorted_sol)
	wHatSol = []
	for i in sorted(qubo_solution):
		#print(el)
		wHatSol.append(1 if qubo_solution[i] == 1 else 0)
	return wHatSol


def AnaliticSol(x, y):
	a = np.dot(x.T, x)
	a_inv = np.linalg.inv(a)
	b = np.dot(x.T, y)
	res = np.dot(a_inv, b)
	return res


def MSE(y1, y2, n):
	return np.sum((y1 - y2) ** 2) / n


def dwave_anneal(qubo):
	sampler = neal.SimulatedAnnealingSampler()
	sampleset = sampler.sample_qubo(qubo)
	qubo_solution = sampleset.first.sample
	return qubo_solution


def quantum_anneal(qubo, pref, Nr):
	"""
	:type qubo: dictionary
	"""
	chain_strength = partial(uniform_torque_compensation, prefactor=pref)
	sampler = EmbeddingComposite(DWaveSampler())
	sampleset = sampler.sample_qubo(qubo, num_reads=Nr, chain_strength=chain_strength)
	qubo_solution = sampleset.first.sample
	inspector.show(sampleset)
	return qubo_solution


def sol_to_label(qubo_solution, n, k):
	s = []
	for el in qubo_solution:
		s.append(qubo_solution[el])
	sol = []
	for i in range(n):
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

	Flab = np.zeros((n, 1))
	for i, row in enumerate(sol):
		if tuple(row) in code:
			Flab[i] = code.index(tuple(row))
		else:
			Flab[i] = -1

	return sol, Flab

class objectHandler:

	def __init__(self, n, d, slope, intercept, noise, nPow, x, y, A, b, qubo, AD, NA, wHatSol, wsol, yFound, twop, P):
		self.n = n
		self.d = d
		self.slope = slope
		self.intercept = intercept
		self.noise = noise
		self.nPow = nPow
		self.x = x
		self.y = y
		self.A = A
		self.b = b
		self.qubo = qubo
		self.AD = AD
		self.NA = NA
		self.wHatSol = wHatSol
		self.wsol = wsol
		self.yF = yFound
		self.twop = twop
		self.P = P
		
