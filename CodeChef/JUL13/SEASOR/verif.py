#!/usr/bin/python

IN = [line.strip() for line in open("input")]
OUT = [line.strip() for line in open("output")]

T = int(IN[0])

for test in range(T):
    N, K = [int(x) for x in IN[1 + 2 * test].split()]
    A = [int(x) for x in IN[2 + 2 * test].split()]

    ANSWER = int(OUT[2 * test])
    V = [int(x) for x in OUT[1 + 2 * test].split()]

    V = V[:ANSWER]
    for x in V:
        x -= 1
        A[x:x + K] = sorted(A[x:x + K])

    if A != sorted(A):
        exit(-1)

exit(0)
