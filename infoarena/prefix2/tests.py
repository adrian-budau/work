COMMANDS = [
    "./gen 100 20",
    "./gen 1000 20",
    "./gen 10000 20",
    "./gen 10000 100",
    "./gen 10000 400",
    "./gen 30000 400",
    "./gen 50000 1000",
    "./gen 100000 1000",
    "./gen 100000 5000",
    "./gen 100000 15000"
]

from subprocess import call

for test in range(1, 11):
    call(COMMANDS[test - 1] + " > prefix2.in", shell=True)
    call("./main", shell=True)

    call("cp prefix2.in grader_test%d.in" % test, shell=True)
    call("cp prefix2.out grader_test%d.ok" % test, shell=True)
    print("Done with test %d" % test)
