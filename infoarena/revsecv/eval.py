TESTS = int(input("TESTS = "))
MAXN = int(input("N = "))

from random import randint
from subprocess import check_call

SOURCES = ["brut"]
def result(source):
    check_call(["./" + source])
    return int([line for line in open("revsecv.out")][0])

for t in range(1, TESTS + 1):
    print("Case #%d: " % t, end="")
    N = randint(1, MAXN)
    if t == TESTS:
        N = MAXN
    check_call(["./generator", str(N)])
    results = dict()
    for source in SOURCES:
        results[source] = result(source)

    if len(set(results.values())) != 1:
        print("Wrong answer")
        for source in SOURCES:
            print("%s -> %d" % (source, results[source]))
        break
    print("OK -> %d" % list(results.values())[0])



