from subprocess import call
from random import randint
import sys

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXV = int(input("MAXV = "))
MAXSHOW = int(input("MAXSHOW = "))

for test in range(1, TESTS + 1):
    N, M = MAXN, MAXM
    if test < TESTS:
        M = randint(1, MAXM)
        N = randint(1, min(M, MAXN))
    SEED = randint(0, 2**32 - 1)
    print("Case #%d(N = %d M = %d SEED = %d): " % (test, N, M, SEED), end="")
    sys.stdout.flush()

    if call("./gen %d %d %d %d %d > spectacole.in" % (N, M, MAXV, MAXSHOW, SEED), shell=True):
        print("Error generating")
        break

    SOURCES = ["main", "brut50"]

    RESULTS = []
    for SOURCE in SOURCES:
        if call("./" + SOURCE, shell=True):
            print("Error running " + SOURCE)
            break
        OUTPUT = [line for line in open("spectacole.out")]
        RESULTS += [int(OUTPUT[0].strip())]

    if len(RESULTS) != len(SOURCES):
        break

    if len(set(RESULTS)) != 1:
        print("Wrong answer")

        for i in range(len(SOURCES)):
            print("%s -> %s" % (SOURCES[i], RESULTS[i]))
        break
    print("OK %d" % RESULTS[0])
