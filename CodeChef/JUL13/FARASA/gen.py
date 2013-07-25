#!/usr/bin/python
N = int(input())

print(N)
print(" ".join([str((4 * 10**10) // (N * N)) for x in range(N)]))
