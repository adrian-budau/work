from subprocess import call
from random import randint

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXV = int(input("MAXV = "))

for test in range(1, TESTS + 1):
    N, M, V = MAXN, MAXM, MAXV
    if test < TESTS:
        N = randint(1, MAXN)
        M = randint(N, MAXM)
        V = randint(1, min(M, MAXV))
    SEED = randint(1, 2**32 - 1)

    print("Case #%d (N = %d M = %d V = %d SEED = %d): " % (test, N, M, V, SEED), end="")
    if call("./gen %d %d %d %d > kthvalue.in" % (N, M, V, SEED), shell=True):
        print("Error with generator")
        break

    if call("./main", shell=True):
        print("Error with main")
        break

    MAIN = [line.strip() for line in open("kthvalue.out")]

    if call("./main2", shell=True):
        print("Error with main2")
        break

    BRUT = [line.strip() for line in open("kthvalue.out")]

    if MAIN != BRUT:
        print("Wrong answer")
        break
    print("OK")
