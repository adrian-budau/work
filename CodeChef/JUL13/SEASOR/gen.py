#!/usr/bin/python

import sys
import random

T, MAXN, MAXV = [int(x) for x in sys.argv[1:]]

print(T)

for _ in range(T):
    N = None
    if _ == T - 1:
        N = MAXN
    else:
        N = random.randint(2, MAXN)

    K = random.randint(2, MAXN)
    print(N, K)
    print(" ".join([str(random.randint(1, MAXV)) for x in range(N)]))

