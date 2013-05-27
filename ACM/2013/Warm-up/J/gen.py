from random import randint
import sys

N = int(sys.argv[1])
MAXV = int(sys.argv[2])

A = []
for i in range(N):
    X = randint(2, MAXV)
    while X in A:
        X = randint(2, MAXV)
    A += [X]

B = []
for i in A:
    for j in A:
        if i < j:
            B += [i * j]

print("1")
print(N)

print(" ".join([str(x) for x in B]))

ok = open("ok", "w")
print("Case 1:", " ".join([str(x) for x in sorted(A)]), file=ok)
