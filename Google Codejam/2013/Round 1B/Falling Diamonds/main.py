#!/usr/bin/python
def completed(N):
    A, add, level = 1, 1, 0
    while A < N:
        add += 4
        if A + add > N:
            return level, N - A
        A += add
        level += 1
    return level, 0


MAXN = 3000
comb = [[0] * MAXN for i in range(MAXN)]
comb[0][0] = 1
for i in range(MAXN - 1):
    for j in range(MAXN - 1):
        comb[i + 1][j] += comb[i][j]
        comb[i + 1][j + 1] += comb[i][j]

from decimal import *
T = int(input())
for test in range(1, T + 1):
    N, X, Y = [int(x) for x in input().split()]

    LEVELS, LEFT = completed(N)
    LEVELS *= 2

    ME = abs(X) + abs(Y)
    answer = None
    if ME <= LEVELS:
        answer = Decimal(1)
    elif ME > LEVELS + 2:
        answer = Decimal(0)
    elif X == 0 or LEFT == 0:
        answer = Decimal(0)
    else:
        need = Y + 1

        total = 0
        good = 0
        for i in range(LEFT + 1):
            j = LEFT - i
            if i > LEVELS + 2 or j > LEVELS + 2:
                continue
            total += comb[LEFT][i]
            if i >= need:
                good += comb[LEFT][i]

        answer = Decimal(good) / Decimal(total)

    print("Case #" + str(test) + ":", answer)

