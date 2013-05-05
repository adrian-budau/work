def interpret(OUT):
    return [line.strip() for line in OUT]

def gen(N, M, MAXTYPE):
    IN = open('input', 'w')

    TYPE = [None for _ in range(N + 1)]
    KEYS = [[] for _ in range(N + 1)]

    if random.randrange(15) <= 15: # we want impossible
        for i in range(1, N + 1):
            TYPE[i] = random.randrange(1, MAXTYPE + 1)

        for i in range(M):
            KEYS[random.randrange(0, N + 1)] += [random.randrange(1, MAXTYPE + 1)]
    else:
        for i in range(1, N + 1):
            TYPE[i] = random.randrange(1, MAXTYPE + 1)
            KEYS[random.randrange(0, i)] += [TYPE[i]]

        for i in range(N, M):
            KEYS[random.randrange(0, N + 1)] += [random.randrange(1, MAXTYPE + 1)]

    CHESTS = [(TYPE[x], KEYS[x]) for x in range(N + 1)]

    AUX = CHESTS[1:]
    random.shuffle(AUX)
    CHESTS[1:] = AUX

    print("1", file=IN)
    print("%d %d" % (len(KEYS[0]), N), file=IN)
    print(" ".join([str(x) for x in KEYS[0]]), file = IN)
    for i in range(1, N + 1):
        print("%d %d %s" % (CHESTS[i][0], len(CHESTS[i][1]), " ".join([str(x) for x in CHESTS[i][1]])), file = IN)

TESTS = int(input("Number of tests ? "))
MAXN = int(input("Max N = "))
MAXM = int(input("Max Keys = "))
MAXTYPE = int(input("Max key value = "))

import random
from subprocess import call as call

for i in range(1, TESTS + 1):
    if i == TESTS:
        gen(MAXN, MAXM, MAXTYPE)
    else:
        N = random.randrange(1, MAXN + 1)
        M = random.randrange(N, MAXM + 1)
        gen(N, M, MAXTYPE)

    MAIN = 0
    assert call("./main < input > output", shell=True) == 0, "Main source crashed"
    with open('output') as OUT:
        MAIN = interpret(OUT)
    call("rm -rf output", shell=True)

    BRUT = 0
    assert call("./brut < input > output", shell=True) == 0, "Brute-force crashed"
    with open('output') as OUT:
        BRUT = interpret(OUT)
    call("rm -rf output", shell=True)

    print("Case #" + str(i) + ":", end="")
    if MAIN != BRUT:
        print(("WRONG!   ", MAIN, "   ", BRUT))
        break
    print("OK!", MAIN)

