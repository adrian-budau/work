#!/usr/bin/python
from sys import argv
from random import randint

N = int(argv[1])
M = int(argv[2])
MAXV = int(argv[3])

A = [[randint(-MAXV, MAXV) for x in range(M)] for x in range(N)]

inp = open("treid.in", "w")

print(N, M, file=inp)

for i in range(N):
    for j in range(M):
        print(A[i][j], file=inp, end=" ")
    print(file=inp)
