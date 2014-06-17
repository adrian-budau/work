from subprocess import call
from random import randint

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXK = int(input("MAXK = "))

for test in range(1, TESTS + 1):
    N, M, K = None, None, None
    if test < TESTS:
        N = randint(1, MAXN)
        M = randint(1, MAXM)
        K = randint(1, MAXK)
    else:
        N, M, K = MAXN, MAXM, MAXK

    if call("./generator " + str(N) + " " + str(M) + " " + str(K) + " > ninja.in", shell=True) != 0:
        print("Generator crashed on test", test)
        break

    if call("./main", shell=True) != 0:
        print("Main source crashed on test", test)
        break

    MAIN = [int(line.strip()) for line in open("ninja.out")]

    if call("./brut", shell=True) != 0:
        print("Brute-force source crashed on test", test)
        break

    BRUT = [int(line.strip()) for line in open("ninja.out")]

    if MAIN != BRUT:
        print("Different output on test", test)
        break

    print("Case #", test, ": OK", sep="")
