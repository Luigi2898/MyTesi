from projectq.ops import H, Measure, All, Ph, C, SwapGate, Command
from projectq import MainEngine
from projectq.backends import Simulator
from numpy import pi

# create a main compiler engine
eng = MainEngine(backend=Simulator())

qKey = eng.allocate_qureg(1)
qValue = eng.allocate_qureg(4)
# put it in superposition

All(H) | qKey
All(H) | qValue

C(Ph(5*pi/8)) | (qKey, qValue[0])
C(Ph(5*pi/4)) | (qKey, qValue[1])
C(Ph(5*pi/2)) | (qKey, qValue[2])
C(Ph(5*pi)) | (qKey, qValue[3])

# inverse QFT
# qValue.set_interchangeable_qubit_indices([1, 2])
SwapGate | (qValue[1], qValue[2])
SwapGate | (qValue[0], qValue[3])

H | qValue[0]

C(Ph(-pi/2)) | (qValue[0], qValue[1])
H | qValue[1]

C(Ph(-pi/2)) | (qValue[1], qValue[2])
C(Ph(-pi/4)) | (qValue[0], qValue[2])
H | qValue[2]

C(Ph(-pi/2)) | (qValue[3], qValue[2])
C(Ph(-pi/4)) | (qValue[3], qValue[1])
C(Ph(-pi/8)) | (qValue[3], qValue[0])
H | qValue[3]

# measure
All(Measure) | qKey
All(Measure) | qValue



eng.flush()

# sim = Simulator()

# ret = sim.get_probability(['1', '0', '1', '0'], qValue)

# print(ret)
# print the result:
print(eng.backend.get_probability(['1', '0', '1', '0'], qValue))
print(f"Key: {[int(qubit) for qubit in qKey]}")
print(f"Value: {[int(qubit) for qubit in qValue]}")