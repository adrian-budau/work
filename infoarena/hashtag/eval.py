TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXP1 = float(input("MAXP1 = "))
MAXP2 = float(input("MAXP2 = "))

from subprocess import call
from random import randint, uniform

def RUN(source):
    call("./%s" % source, shell=True)
    OUTPUT = [line.strip() for line in open("hashtag.out")]
    return int(OUTPUT[0])

for t in range(1, TESTS + 1):
    N = randint(5, MAXN)
    M = randint(5, MAXN)
    P1 = uniform(0, MAXP1)
    P2 = uniform(0, MAXP2)
    K = randint(1, (min(N, M) - 3) // 2)

    call("./gen %d %d %d %f %f > hashtag.in" % (N, M, K, P1, P2), shell=True)

    N8 = RUN("brut")
    N6 = RUN("100")

    print("Case #%d: " % t, end="")
    if N8 != N6:
        print("Wrong Answer")
        break
    else:
        print("OK")
