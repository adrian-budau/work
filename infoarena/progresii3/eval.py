TESTS = int(input("TESTS = "))
MAXV = int(input("MAXV = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXDISTANCE = int(input("MAXDISTANCE = "))

from subprocess import call
from random import randint

for test in range(1, TESTS + 1):
    V, N, K, DISTANCE = None, None, None, None

    if test == TESTS:
        V, N, K, DISTANCE = MAXV, MAXN, MAXK, MAXDISTANCE
    else:
        V = randint(2, MAXV)
        N = randint(2, min(MAXN, V))
        DISTANCE = randint(1, MAXDISTANCE)
        K = randint(1, min(V, MAXK, DISTANCE + 1))

    SEED = randint(0, 2**32 - 1)
    print("Case #%s - V = %s, N = %s, K = %s, DISTANCE = %s, SEED = %s: " % (test, V, N, K, DISTANCE, SEED), end="")
    if call("./gen %s %s %s %s %s > progresii3.in" % (V, N, K, DISTANCE, SEED), shell=True) != 0:
        print("Erorr generating")
        break

    SOURCES = ["main", "brut20"]
    RESULTS = []
    for SOURCE in SOURCES:
        if call("./%s" % SOURCE, shell=True) != 0:
            print("Error running %s" % SOURCE)
            break
        RESULT = int([line for line in open("progresii3.out")][0].strip())
        RESULTS += [RESULT]

    if len(set(RESULTS)) != 1:
        print("Wrong answer:")
        for source, result in zip(SOURCES, RESULTS):
            print("   %10s -> %s" % (source, result))
        break

    print("OK %s" % RESULTS[0])
