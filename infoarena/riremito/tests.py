TESTS = [
    "5 5 5 0",
    "15 1000 10 0",
    "20 1000000000 10 0.5",
    "50 1000000000 10 0",
    "1500 1000000000 10 0",
    "15000 1000000000 10 0.3",
    "40000 1000000000 10 0.5",
    "80000 1000000000 10 0.93",
    "100000 1000000000 10 0.25",
    "100000 1000000000 10 0.7"
]

from subprocess import call
for test in range(10):

    call("./gen %s > riremito.in" % TESTS[test], shell=True)
    call("./main", shell=True)
    call("cp riremito.in grader_test%d.in" % (test + 1), shell=True)
    call("cp riremito.out grader_test%d.ok" % (test + 1), shell=True)

    print("Done with test: %d" % (test + 1))
