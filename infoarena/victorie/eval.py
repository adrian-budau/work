TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXCOMPONENTS = int(input("MAXCOMPONENTS = "))
MAXBICONNECTED = int(input("MAXBICONNECTED = "))
MAXITERATIONS = int(input("MAXITERATIONS = "))

from random import randint
from subprocess import call

def output():
    return [line.strip() for line in open("victorie.out")]

for i in range(1, TESTS + 1):
    N, M, COMPONENTS, BICONNECTED, ITERATIONS = MAXN, MAXM, MAXCOMPONENTS, MAXBICONNECTED, MAXITERATIONS
    if i < TESTS:
        N = randint(2, MAXN)
        M = randint(N, MAXM)
        BICONNECTED = randint(1, MAXBICONNECTED)
        COMPONENTS = randint(1, min(MAXCOMPONENTS, BICONNECTED))
        ITERATIONS = randint(1, MAXITERATIONS)

    SEED = randint(0, 2**32 - 1)
    print("Case #%d (N = %d M = %d K = %d B = %d I = %d Seed = %d): " % (i, N, M, COMPONENTS, BICONNECTED, ITERATIONS, SEED), end="")
    if call("./gen %d %d %d %d %d %d > victorie.in" % (N, M, COMPONENTS, BICONNECTED, ITERATIONS, SEED), shell=True) != 0:
        print("Error generating")
        break
    if call("./main", shell=True) != 0:
        print("Error with main")
        break
    MAIN = output()

    if call("./vladii", shell=True) != 0:
        print("Error with vladii")
        break
    VLADII = output()
    if MAIN != VLADII:
        print("Wrong answer!")
        break
    print("OK!")

