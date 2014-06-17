from sys import argv

R = int(argv[1])
N = int(argv[2])
M = int(argv[3])

INPUT = open("input", "w")

print(R, N, M, file=INPUT)
for op in range(R):
    from random import randint
    X, Y = randint(0, N - 1), randint(0, N - 1)
    if Y < X:
        X, Y = Y, X
    print (randint(0, N - 1), randint(0, 10**9), X, Y, file=INPUT)

