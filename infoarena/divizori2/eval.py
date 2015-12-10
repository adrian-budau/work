TESTS = int(input("TESTS = "))
MAXA = int(input("MAXA = "))
MAXK = int(input("MAXK = "))
MAXD = int(input("MAXD = "))

from random import randint
from subprocess import call

def output():
    return int([line.strip() for line in open("divizori2.out")][0])

for test in range(1, TESTS + 1):
    A = MAXA #A = randint(1, MAXA)
    K = MAXK #K = randint(1, MAXK)
    D = MAXD #D = randint(1, MAXD)

    DIVS = []
    for i in range(2, D + 1):
        while D % i == 0:
            DIVS += [i]
            D /= i

    print("Case #%d: " % test, end="")

    if call("./gen %d %d %s > divizori2.in" % (A, K, " ".join([str(x) for x in DIVS])), shell=True) != 0:
        print("Generator failed")
        break

    if call("./brut") != 0:
        print("Brut failed")
        break

    BRUT = output()

    if call("./main") != 0:
        print("Main failed")
        break

    MAIN = output()

    if MAIN != BRUT:
        print("Wrong Answer")
        break
    print("OK")
