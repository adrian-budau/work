from common import run, generate, RunError
from random import randint
from math import sqrt

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXVALUES = int(input("MAXVALUES = "))
SMALL_VALUES = int(input("SMALL_VALUES = "))

programs = ["brut", "main", "h-100"]

for test in range(1, TESTS + 1):
    print("Case #", test, ":", sep="", end=" ")

    N, M = None, None
    if test == TESTS:
        N, M = MAXN, MAXM
    else:
        N = randint(1, MAXN)
        M = randint(1, MAXM)

    PROBABILITY = 1.0 / sqrt(N)

    try:
        generate(N=N, M=M, PROBABILITY=PROBABILITY, MAXVALUES=MAXVALUES, SMALL_VALUES=SMALL_VALUES)
        results = [(program, run(program)) for program in programs]
        real_result = results[0][1]

        text = ""
        for person in results:
            if person[1] != real_result:
                text += "for " + person[0]
        if text != "":
            raise RunError("Wrong answer " + text)

        zeroes = [int(x) for x in real_result].count(0)
        print("OK", zeroes, "of", M)
    except RunError as e:
        print(e)
        break

