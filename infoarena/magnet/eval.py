TESTS = int(input("TESTS = "))
MAXT = int(input("MAXT = "))
MAXN = int(input("MAXN = "))
MAXV = int(input("MAXV = "))
MAXK = int(input("MAXK = "))

import os
from random import randint
from subprocess import check_call, check_output

SOURCES = ["main"]

def result(source):
    check_call(["./" + source])
    with open(os.devnull, 'w') as fnull:
        return int(check_output(["./grader_eval"], stderr=fnull))

for t in range(1, TESTS + 1):
    SEED = randint(0, 2**32 - 1)
    print("Case #%d(Seed = %d): " % (t, SEED), end="")
    T = randint(1, MAXT)
    T = MAXT
    check_call(["./gen", str(T), str(MAXN), str(MAXV), str(MAXK), str(SEED)], stdout=open("magnet.in", "w"))
    # Generate ok
    check_call(["./main"])
    check_call(["mv", "magnet.out", "magnet.ok"])
    good = True
    for source in SOURCES:
        if result(source) != 10:
            print("Wrong answer -> " + source)
            good = False
    if not good:
        break
    print("OK")



