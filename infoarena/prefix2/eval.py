from random import randint
from subprocess import call

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))

for test in range(1, TESTS + 1):
    N = MAXN
    if test < TESTS:
        N = randint(1, MAXN)

    MAXK = randint(1, N)

    print("Case #%d: " % test, end="")

    if call("./gen %d %d > prefix2.in" % (N, MAXK), shell=True) != 0:
        print("Error generating")
        break

    if call("./main") != 0:
        print("Error with main")
        break

    MAIN = [line.strip() for line in open("prefix2.out")]

    if call("./brut") != 0:
        print("Error with brut")
        break

    BRUT = [line.strip() for line in open("prefix2.out")]

    if MAIN != BRUT:
        print("Wrong Answer\n")
        break

    print("OK")
