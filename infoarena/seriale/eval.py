import random
import subprocess
import pprint

SOURCES = ["brut60", "back"]

TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXLIS = int(input("MAXLIS = "))

def result(name):
    subprocess.check_call(["rm", "-f", "seriale.out"])
    subprocess.check_call(["./" + name])
    return int([line.strip() for line in open("seriale.out")][0])

for test in range(1, TESTS + 1):
    N = random.randint(1, MAXN)
    K = random.randint(1, MAXK)
    LIS = random.randint(1, min(MAXLIS, N))
    if test == TESTS:
        N = MAXN
        K = MAXK
        LIS = MAXLIS

    LENGTH = random.randint(N, N + K)
    START = random.randint(1, N + K - LENGTH + 1)
    END = START + LENGTH - 1

    subprocess.check_call(["./gen", str(N), str(K), str(START), str(END), str(LIS)], stdout=open("seriale.in", "w"))

    results = {}
    for source in SOURCES:
        results[source] = result(source)

    print("Case %d (%d %d %d %d %d): " % (test, N, K, START, END, LIS), end="")
    if len(set(results.values())) != 1:
        print("Wrong Answer")
        pprint.pprint(results)
        break
    print("OK with %d" % list(results.values())[0])
