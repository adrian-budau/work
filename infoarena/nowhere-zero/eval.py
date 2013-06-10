#!/usr/bin/python
import sys
import random
from subprocess import call,TimeoutExpired

TESTS, MAXBEGIN, MAXEND, CMAX = None, None, None, None
if len(sys.argv) > 1:
    TESTS, MAXBEGIN, MAXEND, CMAX = [int(x) for x in sys.argv]
else:
    TESTS = int(input("TESTS = "))
    MAXBEGIN = int(input("MAXBEGIN = "))
    MAXEND = int(input("MAXEND = "))
    CMAX = int(input("CMAX = "))

def run(SOURCE):
    if call("./" + SOURCE, shell=True, timeout=5) != 0:
        print("Source", SOURCE, "crashed")
        exit()

for _ in range(TESTS):
    BEGIN, END = None, None
    if _ == TESTS - 1:
        BEGIN, END = MAXBEGIN, MAXEND
    else:
        BEGIN = random.randint(3, MAXBEGIN)
        END = random.randint(BEGIN, MAXEND)
    print("Case #" + str(_ + 1) + ":", end="")

    SEED = random.randint(0, 2**32 - 1)
    print("BEGIN(" + str(BEGIN) + ")", "END(" + str(END) + ")", "SEED(" + str(SEED) + ") ", end="")

    if call("./generator " + str(BEGIN) + " " + str(END) + " " + str(CMAX) + " " + str(SEED) + " > nowherezero.in 2>/dev/null", shell=True, timeout=10) != 0:
        print("Generator crashed\n")
        break

    try:
        run("verif")
    except TimeoutExpired:
        pass
    run("main")
    run("grader_eval > score")
    RESULT = [int(line.strip()) for line in open("score")]
    RESULT = RESULT[0]
    if RESULT != 10:
        print("    Wrong!")
        exit()
    print("  OK   ")
