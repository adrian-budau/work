TESTS = int(input("Number of tests ? "))
MAXN = int(input("Max N = "))

import random
from subprocess import call as call

for i in range(1, TESTS + 1):
    N = random.randrange(1, MAXN + 1)
    M = random.randrange(1, N + 1)

    with open('templu.in', 'w') as IN:
        print(str(N) + " " + str(M), file=IN)

    MAIN = 0
    assert call("./bigdawgs2") == 0, "Main source crashed"
    with open('templu.out') as OUT:
        MAIN = int(OUT.readline())
    call("rm -rf templu.out", shell=True)

    BRUT = 0
    assert call("./brut") == 0, "Brute-force crashed"
    with open('templu.out') as OUT:
        BRUT = int(OUT.readline())
    call("rm -rf templu.out", shell=True)

    print("Case #" + str(i) + ":", end="")
    if MAIN != BRUT:
        print(("WRONG!   ", MAIN, "   ", BRUT))
        break
    print("OK!")

