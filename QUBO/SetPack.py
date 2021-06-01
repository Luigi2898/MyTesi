from dwave.system import EmbeddingComposite, DWaveSampler
from qubovert import boolean_var

N = 4

x = {i: boolean_var('x(%d)' % i) for i in range(N)}

model = 0
for i in range(N):
	model += x[i]

model = - model

model.add_constraint_le_zero(x[0] + x[2] + x[3] - 1, lam=6)

model.add_constraint_le_zero(x[0] + x[1] - 1, lam=6)

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
