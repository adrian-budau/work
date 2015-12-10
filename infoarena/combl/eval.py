from random import randint
from subprocess import call

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXQ = int(input("MAXQ = "))
MAXV = int(input("MAXV = "))

for test in range(1, TESTS + 1):
    print("Case #%d - " % test, end="")

    N = randint(2, MAXN)
    Q = randint(N, MAXQ)
    SEED = randint(0, 2**32 - 1)

    if test == TESTS:
        N, Q = MAXN, MAXQ
    print("N = %d, Q = %d, MAXV = %d, SEED = %d: " % (N, Q, MAXV, SEED), end="")
    if call("./gen_good %d %d %d %d > combl.in" % (N, Q, MAXV, SEED), shell=True) != 0:
        print("Error generating")
        break

    if call("./brut") != 0:
        print("Error with brute-force solution")
        break

    BRUT = [line for line in open("combl.out")]

    if call("./main") != 0:
        print("Error with main solution")
        break

    MAIN = [line for line in open("combl.out")]

    if MAIN != BRUT:
        print("Wrong answer")
        break
    print("OK")

