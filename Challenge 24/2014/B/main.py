import numpy as np

DATA = [line.strip() for line in open("input")]
N, M = [int(x) for x in DATA[0].split()]

A = [[0] * N for x in range(N)]

for i in range(1, M + 1):
    X, Y = [int(x) for x in DATA[i].split()]
    A[X][X] += 1
    A[Y][Y] += 1
    A[X][Y] -= 1
    A[Y][X] -= 1

a = np.array(A).astype(int)
a = np.delete(a, 0, 0)
a = np.delete(a, 0, 1)

total = np.linalg.slogdet(a)[1]

X, Y = [int(x) for x in DATA[1].split()]
A[X][X] -= 1
A[Y][Y] -= 1
A[X][Y] += 1
A[Y][X] += 1

a = np.array(A).astype(int)
a = np.delete(a, 0, 0)
a = np.delete(a, 0, 1)

bad = np.linalg.slogdet(a)[1]

answer = bad - total;
answer = np.exp(answer)

answer = 1 - answer
print(answer)

print("%.9f" % answer, file=open("B10.out", "w"))
