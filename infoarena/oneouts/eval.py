TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXA = int(input("MAXA = "))
MAXB = int(input("MAXB = "))

from subprocess import check_call
from random import randint

SOURCES = ["main", "greedy"]
def result(source):
    check_call(["./" + source])
    return [int(line.strip()) for line in open("oneouts.out")][0]

count = 0
for test in range(1, TESTS + 1):
    print("Case #%d: " % test, end="")

    N = randint(3, MAXN)
    K = randint(1, MAXK)
    if test == TESTS:
        N = MAXN
        K = MAXK

    check_call(["./gen-simple", str(N), str(K), str(MAXA), str(MAXB)], stdout=open("oneouts.in", "w"))
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
    print("OK!")
