TESTS = [
    "5 5 0 10",
    "100 100 1 100",
    "450 1000 -1000000000 1000000000",
    "20000 100000 -1000000000 1000000000",
    "50000 100000 -1000000000 1000000000",
    "100000 50000 -1000000000 1000000000",
    "100000 100000 1 100000",
    "100000 100000 -1000000000 1000000000",
    "100000 100000 -1000000000 1000000000",
    "100000 100000 -1000000000 1000000000"
]

from subprocess import call

for i in range(len(TESTS)):
    call("./gen " + TESTS[i] + " > compunere.in", shell=True)
    call("./main", shell=True)
    call("cp compunere.in grader_test%d.in" % (i + 1), shell=True)
    call("cp compunere.out grader_test%d.ok" % (i + 1), shell=True)
    print("Done with test %d" % (i + 1))
