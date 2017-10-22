import random
import subprocess
import pprint
import os
import io

SOURCES = ["main"]

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))

def result(name):
    subprocess.check_call(["rm", "-f", "puzzle2.out"])
    subprocess.check_call(["./" + name])
    return int(subprocess.check_output(["./grader_eval"], stderr=open(os.devnull, 'w')))

for test in range(1, TESTS + 1):
    N = random.randint(1, MAXN)
    M = random.randint(1, MAXM)
    if test == TESTS:
        N = MAXN
        M = MAXM

    subprocess.check_call(["./generator", str(N), str(M)], stdout=open("puzzle2.in", "w"))

    results = {}
    results["Corect"] = 10
    for source in SOURCES:
        results[source] = result(source)

    print("Case %d (%d %d): " % (test, N, M), end="")
    if len(set(results.values())) != 1:
        print("Wrong Answer")
        pprint.pprint(results)
        break
    print("OK")
