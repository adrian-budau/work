from random import randint
from subprocess import call

TESTS = int(input("TESTS = "))

MAXR = int(input("MAXR = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))

for test in range(1, TESTS + 1):
    print("Case #%d:" % test, end="")

    R = randint(1, MAXR)
    N = randint(1, MAXN)
    M = randint(2, MAXM)

    call("python3 gen.py %d %d %d" % (R, N, M), shell=True)

    if call("./main < input > output", shell=True) != 0:
        print("Main crashed")
        break

    if call("python3 brut.py < input > ok", shell=True) != 0:
        print("Brute-force crashed")
        break

    if call("diff output ok > /dev/null", shell=True) != 0:
        print("Wrong answer")
        break

    print("OK!")
