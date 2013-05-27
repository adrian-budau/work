TESTS = int(input("TESTE = "))

MAXN = int(input("MAXN = "))

MAXV = int(input("MAXV = "))

from random import randint
from subprocess import call

for _ in range(TESTS):
    V = None
    if _ == TESTS - 1:
        V = MAXN
    else:
        V = randint(3, MAXN)
    call("python gen.py " + str(V) + " " + str(MAXV) + " > input", shell=True)
    if call("./main < input > output", shell=True) != 0:
        print(str(_ + 1) + ": Wrong")
        break
    if call("diff output ok", shell=True) != 0:
        print(str(_ + 1) + ": Aiurea")
        break
    print(str(_ + 1) + ": OK")
