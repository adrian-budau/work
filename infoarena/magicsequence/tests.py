from subprocess import call
COMMANDS = [
    "./gen 1 5 10",
    "./gen 5 100 5000",
    "./gen 10 1000 1000",
    "./gen 15 10000 10000",
    "./gen 15 10000 1000000000",
    "./gen 19 15000 1000000000",
    "./gen 18 18000 1000000000",
    "./gen 20 1000 1000000000",
    "./gen 20 10000 1000000000",
    "./gen 20 20000 1000000000"
]

for test in range(10):
    call("%s > magicsequence.in" % COMMANDS[test], shell=True)
    call("./main", shell=True)
    call("cp magicsequence.in grader_test%d.in" % (test + 1), shell=True)
    call("cp magicsequence.out grader_test%d.ok" % (test + 1), shell=True)
    print("Done with test %d" % (test + 1))
