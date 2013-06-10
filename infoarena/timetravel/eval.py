#!/usr/bin/python
import sys
import random
from subprocess import call

TESTS, MAXN, MAXM, MAXV, REFRESH, RATIO = None, None, None, None, None, None
if len(sys.argv) > 1:
    TESTS, MAXN, MAXM, MAXV, REFRESH = [int(x) for x in sys.argv[1:6]]
    RATIO = float(sys.argv[6])
else:
    TESTS = int(input("TESTS = "))
    MAXN = int(input("MAXN = "))
    MAXM = int(input("MAXM = "))
    MAXV = int(input("MAXV = "))
    REFRESH = int(input("REFRESH = "))
    RATIO = float(input("RATIO = "))

def run(SOURCE):
    if call("./" + SOURCE, shell=True, timeout=5) != 0:
        print("Source", SOURCE, "crashed")
        exit()
    return [line.strip() for line in open("timetravel.out")]

for _ in range(TESTS):
    N, M, TYPE = None, None, None
    if _ == TESTS - 1:
        N, M, TYPE = MAXN, MAXM, REFRESH
    else:
        N = random.randint(1, MAXN)
        M = random.randint(1, MAXM)
        REFRESH = random.randint(2, REFRESH)
    print("Case #" + str(_ + 1) + ":", end="")

    SEED = random.randint(0, 2**32 - 1)
    print("SEED(" + str(SEED) + ") ", end="")

    if call("./gen-weak " + str(N) + " " + str(M) + " " + str(MAXV) + " " + str(REFRESH) + " " + str(RATIO) + " " + str(SEED) + " > timetravel.in 2>/dev/null", shell=True) != 0:
        print("Generator crashed\n")
        break

    SOURCES = ["main", "brut"]
    RESULT = []
    SET = set()
    for a in SOURCES:
        RESULT += [run(a)]
        SET.add(" ".join(RESULT[-1]))

    if len(SET) != 1:
        for a, b in zip(SOURCES, RESULT):
            print(a, "->", b[:5])
        break

    print("  OK   ", RESULT[0][:5])
