TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXK = int(input("MAXK = "))
MAXNEGATIVES = int(input("MAXNEGATIVES = "))
MAXHP = int(input("MAXHP = "))
MAXLIMIT = int(input("MAXLIMIT = "))

from random import randint
from subprocess import call

def output():
    return int([line.strip() for line in open("sao.out")][0])


for test in range(1, TESTS + 1):
    N = randint(1, MAXN)
    M = randint(1, min(MAXM, N))
    K = randint(1, MAXK)
    NEGATIVES = randint(0, min(MAXNEGATIVES, N))
    LIMIT = randint(1, min(MAXLIMIT, N))

    print("Case %d(%d): " % (test, N), end="")

    if call("./gen %d %d %d %d %d %d > sao.in" % (N, M, K, NEGATIVES, MAXHP, LIMIT), shell=True) != 0:
        print("Generator failed")
        break

    if call("./greedy") != 0:
        print("Greedy failed")
        break

    BRUT = output()

    if call("./main") != 0:
        print("Main failed")
        break

    MAIN = output()

    if MAIN != BRUT:
        print("Wrong answer")
        break
    print("OK %d" % MAIN)

