TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MINV = int(input("MINV = "))
MAXV = int(input("MAXV = "))

from random import randint
from subprocess import call

def output():
    return [line.strip() for line in open("compunere.out")]

for i in range(1, TESTS + 1):
    N, M = MAXN, MAXM
    if i < TESTS:
        N = randint(2, MAXN)
        M = randint(N, MAXM)

    SEED = randint(0, 2**32 - 1)
    print("Case #%d (N = %d M = %d MINV = %d MAXV = %d Seed = %d): " % (i, N, M, MINV, MAXV, SEED), end="")
    if call("./gen %d %d %d %d %d > compunere.in" % (N, M, MINV, MAXV, SEED), shell=True) != 0:
        print("Error generating")
        break
    if call("./main", shell=True) != 0:
        print("Error with main")
        break
    MAIN = output()

    if call("./brut", shell=True) != 0:
        print("Error with brut")
        break
    BRUT = output()
    if MAIN != BRUT:
        print("Wrong answer!")
        break
    print("OK!")

