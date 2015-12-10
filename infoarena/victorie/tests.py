TESTS = [
    "5 5 1 1 40",
    "100 100 5 10 50",
    "250 250 1 100 50",
    "5000 10000 5 20 10",
    "100000 250000 1 1 40",
    "100000 250000 1 50 100",
    "100000 250000 10 100 20",
    "100000 250000 100 2500 150",
    "100000 250000 100 5000 20",
    "100000 250000 100 5000 5"
]

from subprocess import call

for i in range(len(TESTS)):
    call("./gen " + TESTS[i] + " > victorie.in", shell=True)
    call("./main", shell=True)
    call("cp victorie.in grader_test%d.in" % (i + 1), shell=True)
    call("cp victorie.out grader_test%d.ok" % (i + 1), shell=True)
    print("Done with test %d" % (i + 1))
