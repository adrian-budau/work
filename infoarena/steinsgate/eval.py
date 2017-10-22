TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXPARTS = int(input("MAXPARTS = "))
MAXOUTER = int(input("MAXOUTER = "))
MAXMINSIZE = int(input("MAXMINSIZE = "))
MAXVALUE = int(input("MAXVALUE = "))

from random import randint
from subprocess import check_call

SOURCES = ["main", "greedy"]
def result(source):
    check_call(["./" + source])
    return [line for line in open("steinsgate.out")][0].strip()


for test in range(1, TESTS + 1):
    MINSIZE = randint(2, MAXMINSIZE)
    N = randint(2 * MINSIZE, MAXN)
    K = randint(1, MAXK)
    PARTS = randint(1, min(N // (2 * MINSIZE), MAXPARTS))
    OUTER = randint(1, min(PARTS, MAXOUTER))

    if test == TESTS:
        MINSIZE = MAXMINSIZE
        N = MAXN
        K = MAXK
        PARTS = MAXPARTS
        OUTER = MAXOUTER
    check_call(["./gen", str(N), str(K), str(PARTS), str(OUTER), str(MINSIZE), str(MAXVALUE)], stdout=open("steinsgate.in", "w"))

    results = dict()
    for source in SOURCES:
        results[source] = result(source)

    if len(set(results.values())) != 1:
        print("Wrong answer")
        for source in SOURCES:
            to_print = results[source]
            if len(to_print) > 25:
                to_print = to_print[:25] + ' ...'
            print("%s -> %s" % (source, to_print))
        break
    print("OK -> %s" % list(results.values())[0][:25])


