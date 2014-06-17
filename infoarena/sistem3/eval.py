from subprocess import call
from random import randint

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXCYCLE = int(input("MAXCYCLE = "))
MAXMODULO = int(input("MAXMODULO = "))

for test in range(1, TESTS + 1):
    print("Case #", test, ": ", sep="", end="")

    N, CYCLE, MODULO = None, None, None
    if test == TESTS:
        N, CYCLE, MODULO = MAXN, MAXCYCLE, MAXMODULO
    else:
        N = randint(1, MAXN)
        CYCLE = randint(1, min(N, MAXCYCLE))
        MODULO = randint(2, MAXMODULO)

    if call("./generator " + str(N) + " " + str(CYCLE) + " " + str(MODULO) + " > sistem3.in", shell=True) != 0:
        print("Generator crashed, WTH?")
        break

    if call("./main", shell=True) != 0:
        print("Problem problem")
        break

    if call("./grader_eval > result 2> /dev/null ", shell=True) != 0:
        print("Verificator problem, WTH?")
        break

    points = int([line for line in open("result")][0])

    if points != 10:
        print("Finally works, but yet no points")
        break

    print("OK")
