TESTS = int(input("TESTS = "))
MAXN = int(input("MAXN = "))
MAXM = int(input("MAXM = "))
MINV = int(input("MINV = "))
MAXV = int(input("MAXV = "))
MAXR = int(input("MAXR = "))

from random import randint
from subprocess import check_call, check_output

SOURCES = ["brut80", "main"]

for test in range(1, TESTS + 1):
    N, M = MAXN, MAXM
    if test < TESTS:
        N, M = randint(2, MAXN), randint(2, MAXM)
    print("Case #%d: " % test, end="")
    check_call(["./gen", str(N), str(M), str(MINV), str(MAXV), str(MAXR)], stdout=open("danger.in", "w"))
    check_call(["./main"])
    check_call(["cp", "danger.out", "danger.ok"])

    breaked = False
    for source in SOURCES:
        check_call(["./" + source])
        score = int(check_output(["./grader_eval"], stderr=open("/dev/null", "w")))
        if score != 5:
            print("Wrong answer %s" % source)
            breaked = True
            break
    if breaked:
        break
    print("OK")
