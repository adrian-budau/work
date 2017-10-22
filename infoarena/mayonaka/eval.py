TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXSTEP = int(input("MAXSTEP = "))
MAXVALUE = int(input("MAXVALUE = "))

from random import randint
from subprocess import check_call

SOURCES = ["brut2", "brut"]
def result(source):
    check_call(["./" + source])
    return [line for line in open("mayonaka.out")][0].strip()


for test in range(1, TESTS + 1):
    print("Case #%d: " % test, end="")
    N = randint(2, MAXN)
    M = randint(1, MAXM)
    if test == TESTS:
        N = MAXN
        M = MAXM
    K = randint(1, N)
    MINLENGTH = randint(1, N)
    check_call(["./gen", str(N), str(M), str(K), str(MAXSTEP), str(MINLENGTH), str(MAXVALUE)], stdout=open("mayonaka.in", "w"))

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


