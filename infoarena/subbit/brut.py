#!/usr/bin/env python3
S = [line.strip() for line in open("subbit.in", "r")][0]

C = ""
i = 1
while len(C) < 4 * len(S):
    C += bin(i)[2:]
    i += 1

pos = 0
for c in S:
    while C[pos] != c:
        pos += 1
    pos += 1

print(pos, file=open("subbit.out", "w"))
