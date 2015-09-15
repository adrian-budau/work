TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MAXK = int(input("MAXK = "))

from random import randint
from subprocess import call

for test in range(1, TESTS + 1):
    print("Case %d: " % test, end="")
    M = randint(1, MAXM)
    N = randint(1, min(MAXN, M))
    K = randint(M, MAXK)
    X = randint(0, N - 1)
    Y = 0
    if X > 0:
        Y = randint(X, M - (N - X))

    call("./gen %d %d %d %d %d > mostenire.in" % (N, M, K, X, Y), shell=True)
    if call("./main", shell=True) != 0:
        print("Main failed")
        break

    call("cp mostenire.in mostenire.ok", shell=True);
    D = int([line.strip() for line in open("mostenire.out")][0])
    call("echo %d >> mostenire.ok" % D, shell=True)

    if call("./verif") != 0:
        print("Main wrong answer with itself")
        break

    #if call("./main") != 0:
    #    print("Main failed")
    #    break

    #if call("./verif") != 0:
    #    print("Wrong answer main")
    #    break

    if call("./dani") != 0:
        print("Dani failed")
        break

    if call("./verif") != 0:
        AND = int([line.strip() for line in open("mostenire.out")][0])
        print("Wrong answer dani")
        if AND < 5000000:
            break

    print("OK")
