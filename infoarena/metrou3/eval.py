TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXT = int(input("MAXT = "))

from random import randint
from subprocess import call

def run(source):
    call("./%s" % source, shell=True)
    OUTPUT = [line.strip() for line in open("metrou3.out")]
    return int(OUTPUT[0])

for t in range(1, TESTS + 1):
    N, T = MAXN, MAXT
    if t < TESTS:
        N = randint(3, MAXN)
        T = randint(N, MAXT)

    call("./gen %d %d > metrou3.in" % (N, T), shell=True)

    print("Case #%d: " % t, end="")
    MAIN = run("main")
    #BRUT = run("brut")
    BRUT40 = run("brut40")

    if MAIN != BRUT40: # or MAIN != BRUT:
        print("Wrong Answer: %d %d" % (BRUT40, MAIN))
        break
    print("OK")
