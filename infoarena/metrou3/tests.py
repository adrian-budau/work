TESTS = [
    "5 10",
    "750 10000000",
    "750 100000000",
    "10000 100000000",
    "15000 20000",
    "20000 100000000",
    "40000 100000000",
    "60000 100000000",
    "80000 100000000",
    "100000 100000000"
]

from subprocess import call

for i in range(len(TESTS)):
    call("./gen %s > metrou3.in" % TESTS[i], shell=True)
    call("./main", shell=True)
    call("cp metrou3.in grader_test%d.in" % (i + 1), shell=True)
    call("cp metrou3.out grader_test%d.ok" % (i + 1), shell=True)

    print("Done with test %d" % (i + 1))

