from subprocess import call
from random import randint

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))

for test in range(1, TESTS + 1):
    N, M = MAXN, MAXM
    if test < TESTS:
        N, M = randint(2, MAXN), randint(1, MAXM)
    SEED = randint(0, 2**32 - 1)

    print("Case #%d (N = %d M = %d SEED = %d): " % (test, N, M, SEED), end="")

    #if call("./gen-good %d %d 10 8 %d > disconnect.in" % (N, M, SEED), shell=True):
    if call("./gen-bad %d %d %d > disconnect.in" % (N, M, SEED), shell=True):
        print("Error generating test")
        break

    SOURCES = ["main", "brut70"] #, "brut30", "brut40"]
    RESULTS = []

    for SOURCE in SOURCES:
        call("rm -f disconnect.out", shell=True)
        if call("./" + SOURCE, shell=True) != 0:
            print("Error running " + SOURCE)
            break
        OUTPUT = "".join([line for line in open("disconnect.out")])
        RESULTS += [OUTPUT]

    if len(RESULTS) != len(SOURCES):
        break

    if len(set(RESULTS)) != 1:
        print("Wrong Answer")
        for i in range(len(SOURCES)):
            for j in range(i, len(SOURCES)):
                if RESULTS[i] != RESULTS[j]:
                    print("%+10s  <>  %10s" % (SOURCES[i], SOURCES[j]))
        break

    YES = RESULTS[0].split('\n').count('YES')
    TOTAL = len(RESULTS[0].split('\n'))
    PERC = float(YES) / float(TOTAL)

    print("OK " + str(YES) + " " + str(PERC))

