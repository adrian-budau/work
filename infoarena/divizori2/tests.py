TESTS = [
    "3 3 2",
    "5 5 2 3",
    "6 6 2 2 2",
    "7 7 2 2",
    "5 8 4 3",
    "7 10 2 5",
    "5 3 2 2 2 3 2 2 2 3",
    "11 8 2 2 3 2 2",
    "4 4 2 2 2 2 2 2 2 2 2 2",
    "5 2 4 7 2 3 2 5"
]

from subprocess import call

def output():
    return int([line.strip() for line in open("divizori2.out")][0])


for test in range(10):
    while True:
        print("Trying with test %d" % (test + 1))
        call("./gen %s > divizori2.in" % TESTS[test], shell=True)
        call("./main", shell=True)
        MAIN = output()
        call("./greedy", shell=True)
        GREEDY = output()
        if MAIN != GREEDY or test < 3:
            print("With ", MAIN, GREEDY)
            break

    call("cp divizori2.in grader_test%d.in" % (test + 1), shell=True)
    call("cp divizori2.ok grader_test%d.ok" % (test + 1), shell=True)
    print("Done with test %d " % (test + 1))
