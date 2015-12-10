TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXK = int(input("MAXK = "))

from random import randint
from subprocess import call

for test in range(1, TESTS + 1):
    N, M, K = None, None, None
    if test < TESTS:
        N = randint(2, MAXN)
        M = randint(N - 1, min(MAXM, N * (N - 1) / 2))
        K = randint(2, min(MAXK, N))
    else:
        N, M, K = MAXN, MAXM, MAXK

    SEED = randint(0, 2**32 - 1)
    print("Case #%d - N = %s, M = %s, K = %s, SEED = %s: " % (test, N, M, K, SEED), end="")
    if call("./gen %s %s %s %s > treespotting.in" % (N, M, K, SEED), shell=True) != 0:
        print("Error generating test")
        break

    if call("./main", shell=True) != 0:
        print("Error running main")
        break

    MAIN = [line.strip() for line in open("treespotting.out")]

    if call("./brut", shell=True) != 0:
        print("Error running brute-force")
        break

    BRUT = [line.strip() for line in open("treespotting.out")]

    if MAIN != BRUT:
        print("Wrong answer")
        break

    if MAIN[0] == "0":
        print("Wrong answer - 0")
        break

    print("OK", MAIN[0])
