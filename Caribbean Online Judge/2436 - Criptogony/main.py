from string import maketrans
from collections import Counter

T = int(raw_input())

for _ in range(T):
    N, M = [int(x) for x in raw_input().split()]
    A = raw_input().split()

    A = "".join(A)

    S = raw_input()
    B = [X[0] for X in Counter(S).most_common()]
    B = "".join(B)

    if len(B) < len(A):
        B += " " * (len(A) - len(B))

    T = maketrans(B, A)

    print S.translate(T)
