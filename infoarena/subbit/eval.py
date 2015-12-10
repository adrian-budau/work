import random
import subprocess
import pprint

SOURCES = ["main", "brut.py"]

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MINPROB = float(input("MINPROB = "))

def result(name):
    subprocess.check_call(["rm", "-f", "subbit.out"])
    subprocess.check_call(["./" + name])
    return int([line.strip() for line in open("subbit.out")][0])

for test in range(1, TESTS + 1):
    N = random.randint(1, MAXN)
    PROB = random.uniform(MINPROB, 1.0)

    subprocess.check_call(["./gen", str(N), str(PROB)], stdout=open("subbit.in", "w"))

    results = {}
    for source in SOURCES:
        results[source] = result(source)

    print("Case %d (%d %f): " % (test, N, PROB), end="")
    if len(set(results.values())) != 1:
        print("Wrong Answer")
        pprint.print(results)
        break
    print("OK")
