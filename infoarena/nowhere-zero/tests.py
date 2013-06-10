#!/usr/bin/python

class WrongAnswer(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class BadtoGood(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

import sys
import random
from subprocess import call, TimeoutExpired

def run(SOURCE):
    if call("./" + SOURCE, shell=True, timeout=5) != 0:
        print("Source", SOURCE, "crashed")
        exit()
    return [line.strip() for line in open("timetravel.out")]

TESTE = [line.strip().split() for line in open("tests.info") if line[0] != '#']

TESTS = len(TESTE)
for _ in range(TESTS):
    N, M, MAXV, REFRESH = [int(x) for x in TESTE[_][0:4]]
    RATIO = float(TESTE[_][4])
    KILL = int(TESTE[_][5])

    print("Test #" + str(_ + 1) + ":", end="")

    while True:
        try:
            SEED = random.randint(0, 2**32 - 1)
            print("SEED(" + str(SEED) + ") ")
            if call("./gen-weak " + str(N) + " " + str(M) + " " + str(MAXV) + " " + str(REFRESH) + " " + str(RATIO) + " " + str(SEED) + " > timetravel.in 2>/dev/null", shell=True) != 0:
                print("Generator crashed\n")
                exit()

            SET = set()
            RESULT = [run("main")]
            SET.add(" ".join(RESULT[-1]))

            SOURCES = []
            if KILL == 0:
                SOURCES = ['brut', 'brut-prost']
                SET = set()
                for a in SOURCES:
                    RESULT += [run(a)]
                    SET.add(" ".join(RESULT[-1]))

                if len(SET) != 1:
                    raise WrongAnswer("Dau diferit")
            elif KILL == 1:
                SOURCES = ['brut']
                SET = set()
                for a in SOURCES:
                    RESULT += [run(a)]
                    SET.add(" ".join(RESULT[-1]))

                if len(SET) != 1:
                    raise WrongAnswer("Dau diferit")

                RESULT += [run("brut-prost")]
                SET.add(" ".join(RESULT[-1]))
                if len(SET) != 2:
                    raise BadtoGood("brut penal")
            else:
                run("main")
                try:
                    run("brut")
                except TimeoutExpired:
                    pass
                else:
                    raise BadtoGood("Should be slower X(")

        except TimeoutExpired:
            if KILL == 2:
                raise
            continue
        except WrongAnswer:
            if KILL == 1:
                raise
            continue
        except BadtoGood:
            continue
        break

    run("main")
    call("cp timetravel.in grader_test" + str(_ + 1) + ".in", shell=True)
    call("cp timetravel.out grader_test" + str(_ + 1) + ".ok", shell=True)
