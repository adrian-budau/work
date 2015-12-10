from subprocess import call
COMMANDS = [
    "./gen 100 1000 50",
    "./gen 10000 200000 5000",
    "./gen 50000 1000000 1000000",
    "./gen 100000 1000000 1000000",
    "./gen 200000 1000000 1000000"
]

for test in range(5):
    call("%s > kthvalue.in" % COMMANDS[test], shell=True)
    call("./main", shell=True)
    call("cp kthvalue.in grader_test%d.in" % (test + 1), shell=True)
    call("cp kthvalue.out grader_test%d.ok" % (test + 1), shell=True)
    print("Done with test %d" % (test + 1))
