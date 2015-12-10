import random
import subprocess
import pprint

SOURCES = ["main", "brut50-1"]

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXPART = int(input("MAXPART = "))

def result(name):
    subprocess.check_call(["rm", "-f", "infinitepatternmatching.out"])
    subprocess.check_call(["./" + name])
    return int([line.strip() for line in open("infinitepatternmatching.out")][0])

for test in range(1, TESTS + 1):
    N = random.randint(1, MAXN)
    PART = random.randint(0, min(MAXPART, N))
    if test == TESTS:
        N = MAXN
        PART = MAXPART

    subprocess.check_call(["./gen", str(N), str(PART)], stdout=open("infinitepatternmatching.in", "w"))

    results = {}
    for source in SOURCES:
        results[source] = result(source)

    print("Case %d (%d %d): " % (test, N, PART), end="")
    if len(set(results.values())) != 1:
        print("Wrong Answer")
        pprint.pprint(results)
        break
    print("OK")
