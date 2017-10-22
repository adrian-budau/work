import random
import subprocess
import pprint

SOURCES = ["main", "brut"]

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))

def result(name):
    subprocess.check_call(["rm", "-f", "subpermutari.out"])
    subprocess.check_call(["./" + name])
    return int([line.strip() for line in open("subpermutari.out")][0])

for test in range(1, TESTS + 1):
    N = random.randint(1, MAXN)
    K = random.randint(0, min(MAXK, N))
    if test == TESTS:
        N = MAXN
        K = MAXK

    A = random.randint(1, N)
    B = random.randint(1, N)
    if A > B:
        A, B = B, A
    subprocess.check_call(["./generator", str(N), str(K), str(A), str(B)], stdout=open("subpermutari.in", "w"))

    results = {}
    for source in SOURCES:
        results[source] = result(source)

    print("Case %d (%d %d % d %d): " % (test, N, K, A, B), end="")
    if len(set(results.values())) != 1:
        print("Wrong Answer")
        pprint.pprint(results)
        break
    print("OK")
