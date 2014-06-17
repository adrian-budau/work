from common import run, generate, RunError, copyTest
from math import sqrt

TESTS = [
    (10, 10, 10, 5),
    (1000, 1000, 1000, 10),
    (100000, 100000, 1000000000, 5),
    (40000, 40000, 1000000000, 1000000),
    (50000, 50000, 1000000000, 1000000),
    (50000, 50000, 1000000000, 1000000),
    (70000, 70000, 1000000000, 1000000),
    (100000, 100000, 1000000000, 1000000),
    (100000, 100000, 1000000000, 1000000),
    (100000, 100000, 1000000000, 10000000)
]

programs = ["brut", "main", "h-100"]

for i, test in enumerate(TESTS):
    i += 1
    print("Case #", i, ":", sep="", end=" ")

    N, M, MAXVALUES, SMALL_VALUES = test
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
        copyTest(i)
    except RunError as e:
        print(e)
        break

