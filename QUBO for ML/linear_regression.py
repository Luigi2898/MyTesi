import sys
import numpy as np
from qubovert import QUBO, boolean_var, sim
from matplotlib import pyplot as plt
from utils import MSE

np.set_printoptions(linewidth=1000)

NumberOfPoints = int(sys.argv[1])
Dimension = int(sys.argv[2])
Slope = float(sys.argv[3])
Intercept = float(sys.argv[4])
Noise = float(sys.argv[5])
NumberOfPowers = int(sys.argv[6])
AnnealingDuration = int(sys.argv[7])
AnnealingNumber = int(sys.argv[8])

# # # # # # # # # # # #
# Dataset definition  #
# # # # # # # # # # # #

r = np.random.RandomState()
Y = []
X = np.zeros((NumberOfPoints, 2))
x1 = np.linspace(-10, 10, NumberOfPoints)
for i in range(NumberOfPoints):
    e = r.uniform(-Noise, Noise)
    Y.append(Slope * x1[i] + Intercept + e)
    X[i] = np.hstack([x1[i], 1])
Y = np.array(Y)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Model definition: QUBO model is obtained by multiplication of A and b by boolean variables  #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

Powers = np.linspace(-NumberOfPowers + 1, 0, NumberOfPowers)
PrecisionVector = np.hstack((-(2 ** Powers), 2 ** Powers))
PrecisionVector = np.sort(PrecisionVector)
NumberOfPowers = NumberOfPowers * 2
PrecisionVector = np.reshape(PrecisionVector, (NumberOfPowers, 1))
PrecisionMatrix = np.kron(np.identity(Dimension + 1), PrecisionVector.T)
A1 = np.dot(PrecisionMatrix.T, X.T)
A2 = np.dot(A1, X)
A = np.dot(A2, PrecisionMatrix)
b1 = np.dot(PrecisionMatrix.T, X.T)
b = -2 * np.dot(b1, Y)
NumberOfBooleanVar = NumberOfPowers * (Dimension + 1)
wHat = []
for i in range(NumberOfBooleanVar):
    wHat.append(boolean_var(i))
model = QUBO()
for i in range(NumberOfBooleanVar):
    for j in range(NumberOfBooleanVar):
        model += A[j, i] * wHat[i] * wHat[j]
for i in range(NumberOfBooleanVar):
    model += b[i] * wHat[i]
InitialState = {}
for i in range(NumberOfBooleanVar):
    InitialState[i] = 0

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Solving the model -> If the found value is better then the last one,                    #
# the initial state is updated to the found state, this is iterated AnnealingNumber times #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

OldValue = 0
FileName = f"step__{NumberOfPoints}__{Dimension}__{Slope}__{Intercept}__{NumberOfPowers}"
FileName = FileName.replace('.', '_')
with open(f"./{FileName}.step", "w") as of:
    for i in range(AnnealingNumber):
        AnnealingResult = sim.anneal_qubo(model, anneal_duration=AnnealingDuration, initial_state=InitialState)
        QuboSolution = AnnealingResult.best.state
        Value = AnnealingResult.best.value
        wHatSolution = np.array(list(QuboSolution.values()))
        wHatSolution = np.reshape(wHatSolution, (NumberOfBooleanVar, 1))
        w = np.dot(PrecisionMatrix, wHatSolution)
        of.write(f"Iteration {i} -> value: {Value} solution: {w.T}\n")
        if Value <= OldValue:
            OldValue = Value
            InitialState = QuboSolution
            BestSolution = QuboSolution
if OldValue == 0:
	BestSolution = QuboSolution

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Slope is found by multiplying the boolean solution by the precision matrix, #
# then y is computed by multiplying found slope by X                          #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

wHatSolution = np.array(list(BestSolution.values()))
wHatSolution = np.reshape(wHatSolution, (NumberOfBooleanVar, 1))
w = np.dot(PrecisionMatrix, wHatSolution)
yFound = np.dot(X, w)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Some output files are generated:                                                                        #
#     - step file   - Contains results from different iterations of the algorithm                         #
#     - report file - Contains a report with general informations on problem, model, solver and solution  #
#     - points file - Contains X, Y and Yfound                                                            #
#     - qubo file   - Contains the QUBO model in key - value pair                                         #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

FileName = f"report__{NumberOfPoints}__{Dimension}__{Slope}__{Intercept}__{NumberOfPowers}"
FileName = FileName.replace('.', '_')
with open(f"./{FileName}.rpt", "w") as of:
	of.write("-- LINEAR REGRESSION REPORT --\n")
	of.write("Problem parameters:\n")
	of.write(f"\t- Number of elements in dataset: {NumberOfPoints}\n")
	of.write(f"\t- A: {Slope}\n")
	of.write(f"\t- B: {Intercept}\n")
	of.write(f"\t- e ~ U({-Noise}, {Noise})\n")
	of.write(f"\t- y = A * x1 + B + e\n")
	of.write(f"\t- Dimensionality of dataset: {Dimension}\n")
	of.write(f"\t- Number of elements in precision array: {NumberOfPowers}\n")
	of.write(f"\t- QUBO terms: {model.num_terms}\n")
	of.write(f"\t- Number of binary variables: {NumberOfBooleanVar}\n\n\n\n")
	of.write(f"\t- Precision powers:\t{Powers}\n")
	of.write(f"\t- Precision array:\t{PrecisionVector}\n\n\n")
	of.write("Solution parameters\n")
	of.write(f"\t- Solution is: \n{QuboSolution}\n")
	of.write(f"\t- Selected weights for w[0] are: \n{[PrecisionVector[i] if wHatSolution[i] == 1 else 0 for i in range(NumberOfPowers)]}\n")
	of.write(f"\t- Selected weights for w[1] are: \n{[PrecisionVector[i] if wHatSolution[NumberOfPowers + i] == 1 else 0 for i in range(NumberOfPowers)]}\n")
	of.write(f"\t- Linear regression solution: {w}\n")
	of.write(f"\t- MSE: {MSE(Y, yFound, NumberOfPoints)}\n")
	of.write(f"\t- Value: {Value}\n")
	of.write("Solver parameters:\n")
	of.write(f"\t- Number of annealing: {AnnealingNumber}\n")
	of.write(f"\t- Duration of annealing: {AnnealingDuration}\n")
FileName = f"points__{NumberOfPoints}__{Dimension}__{Slope}__{Intercept}__{NumberOfPowers}"
FileName = FileName.replace('.', '_')
with open(f"./{FileName}.pnt", "w") as of:
	for i, _ in enumerate(X):
		of.write(f"{X[i, 0]},{Y[i]},{yFound[i]}\n")
FileName = f"qubo__{NumberOfPoints}__{Dimension}__{Slope}__{Intercept}__{NumberOfPowers}"
FileName = FileName.replace('.', '_')
with open(f"./{FileName}.qubo", "w") as of:
	of.write("QUBO matrix:\n\n")
	of.write(str(model.Q))
