TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXP = int(input("MAXP = "))

import os
from random import randint
from subprocess import check_call, check_output

SOURCES = ["main"]

def result(source):
    check_call(["./" + source])
    with open(os.devnull, 'w') as fnull:
        return int(check_output(["./grader_eval"], stderr=fnull))

for t in range(1, TESTS + 1):
    print("Case #%d: " % t, end="")
    N = randint(4, MAXN)
    K = randint(1, min(N - 3, MAXK))
    P = randint(1, min(K, MAXP))
    check_call(["./gen", str(N), str(K), str(P)], stdout=open("romania.in", "w"))

    good = True
    for source in SOURCES:
        if result(source) != 10:
            print("Wrong answer -> " + source)
            good = False
    if not good:
        break
    print("OK")



