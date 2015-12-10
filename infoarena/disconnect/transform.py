#!/usr/local/bin/python3
INPUT = [line for line in open("disconnect.in")]
OUTPUT = [line for line in open("disconnect.out")]

N, M = [int(x) for x in INPUT[0].split()]
print("%d %d" % (N, M))
for line in INPUT[1:N]:
    print(line.strip())

XOR = 0
CURR = 0
for line in INPUT[N:]:
    TYPE, X, Y = [int(x) for x in line.split()]
    print("%d %d %d" % (TYPE, X ^ XOR, Y ^ XOR))
    if TYPE == 2:
        ANSWER = OUTPUT[CURR].strip()
        CURR += 1
        if ANSWER == "YES":
            XOR = X
        else:
            XOR = Y
