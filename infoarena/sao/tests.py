TESTS = [
    "10 2 20 0 10 7",
    "1000 300 1000000000 0 15000000 400",
    "1000 18 1000000000 0 2500000 960",
    "100000 500 1000000000 0 60000 37412",
    "100000 20 1000000000 50000 1000000000 85123",
    "100000 2000 1000000000 50000 1000000000 14367",
    "100000 20 1000000000 50000 1000000000 75129",
    "100000 150 1000000000 50500 1000000000 758",
    "100000 80 1000000000 55000 1000000000 68005"
]

from subprocess import call

def output():
    return int([line.strip() for line in open("sao.out")][0])


for test in range(len(TESTS)):
    while True:
        print("Trying with test %d" % (test + 2))
        call("./gen %s > sao.in" % TESTS[test], shell=True)
        call("./main", shell=True)
        MAIN = output()
        call("cp sao.out sao.ok", shell=True)
        call("./greedy", shell=True)
        GREEDY = output()
        if MAIN != GREEDY or test < 4:
            break

    call("cp sao.in grader_test%d.in" % (test + 2), shell=True)
    call("cp sao.ok grader_test%d.ok" % (test + 2), shell=True)
    print("Done with test %d " % (test + 2))

