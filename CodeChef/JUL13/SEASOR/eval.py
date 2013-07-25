#!/usr/bin/python

import random
from subprocess import call

TESTS = int(input("TESTS = "))
MAXT = int(input("MAXT = "))
MAXN = int(input("MAXN = "))
MAXV = int(input("MAXV = "))

for test in range(1, TESTS + 1):
    T = random.randint(1, MAXT)
    if call("./gen.py " + str(T) + " " + str(MAXN) + " " + str(MAXV) + " > input", shell=True, timeout=2) != 0:
        print("Bad generator")
        exit()

    if call("./main < input > output", shell=True, timeout=2) != 0:
        print("Wrong answer, main crashed")
        exit()

    if call("./verif.py", shell=True) != 0:
        print("Wrong answer, verif says no")
        exit()

    print("Case #" + str(test) + ": OK")
