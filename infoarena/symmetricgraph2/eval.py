TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXC = int(input("MAXC = "))
PROB = float(input("PROB = "))
MAXR = int(input("MAXR = "))

SOURCES = ["main"]

from random import randint
from subprocess import check_call

def result(source):
    check_call(["./" + source])
    return [int(line.strip()) for line in open("symmetricgraph2.out")][0]

for test in range(1, TESTS + 1):
    print("Case #%d: " % test, end="")

    N = randint(2, MAXN)
    K = randint(2, min(N, MAXK))
    if test == TESTS:
        N = MAXN
        K = MAXK
    check_call(["./gen", str(N), str(K), str(MAXC), str(PROB), str(MAXR)], stdout=open("symmetricgraph2.in", "w"))

    results = {}
    current = None
    for source in SOURCES:
        results[source] = result(source)
        current = results[source]

    for source in SOURCES:
        if results[source] != current:
            print("Wrong answer")
            for key, value in results.items():
                print("  %s -> %s" % (key, value))
            exit(0)
    print("OK %d" % (results["main"]))
