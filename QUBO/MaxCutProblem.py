from dwave.system import EmbeddingComposite, DWaveSampler
from qubovert import boolean_var

E = 6
NV = 5
V = ((1, 2), (3), (3, 4), (4), ())

x = {i: boolean_var('x(%d)' % i) for i in range(NV)}

model = 0
for s in range(NV):
	try:
		for e in V[s]:
			model += x[s] + x[e] - 2 * x[s] * x[e]
	except TypeError:
		model += x[s] + x[V[s]] - 2 * x[s] * x[V[s]]

model = - model

qubo = model.to_qubo()

print(qubo.pretty_str())

dwave_qubo = qubo.Q

sampler = EmbeddingComposite(DWaveSampler())
sampleset = sampler.sample_qubo(dwave_qubo, num_reads=100)
print(sampleset)

# Run the problem on the sampler and print the results

qubo_solution = sampleset.first.sample

# convert the qubo solution back to the solution to the model
model_solution = model.convert_solution(qubo_solution)

print("Variable assignment:", model_solution)
print("Model value:", model.value(model_solution))
print("Constraints satisfied?", model.is_solution_valid(model_solution))
