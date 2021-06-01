from dwave.system import EmbeddingComposite, DWaveSampler
from qubovert import boolean_var

E = 6
nV = 5
V = ((1, 2), (3), (3, 4), (4), ())
P = 8

x = {i: boolean_var('x(%d)' % i) for i in range(nV)}

model = 0
for i in range(nV):
	model += x[i]

for s in range(nV):
	try:
		for e in V[s]:
			model += P * (1 - x[s] - x[e] + x[s] * x[e])
	except TypeError:
		model += P * (1 - x[s] - x[V[s]] + x[s] * x[V[s]])

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
