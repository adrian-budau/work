TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXK = int(input("MAXK = "))
MAXEDGE = int(input("MAXEDGE = "))
MINPROB = float(input("MINPROB = "))

from random import randint, uniform
from subprocess import call

def run(source):
    if call(source, shell=True) != 0:
        return []
    OUTPUT = [line.strip() for line in open("riremito.out")]
    return [int(x) for x in OUTPUT]

for test in range(1, TESTS + 1):
    print("Case #%d: " % test, end="")

    N = randint(2, MAXN)
    K = randint(1, min(N, MAXK))
    PROB = uniform(MINPROB, 1)

    if call("./gen %d %d %d %f > riremito.in " % (N, MAXEDGE, K, PROB), shell=True) != 0:
        print("Generator failed")
        break

    BACK = run("./back")
    if len(BACK) == 0:
        print("Backtracking solution failed")
        break
    MAIN = run("./main")
    if len(MAIN) == 0:
        print("Main solution failed")
        break

    if MAIN != BACK:
        print("Wrong answer")
        break
    print("OK")
