from subprocess import call
COMMANDS = [
    "./gen 3 10 20 5",
    "./gen 100 250 1000 50 bla bla", # this needs to be with A = B = 0
    "./gen 100 250 1250 100",
    "./gen 500 2000 1000 50",
    "./gen 500 2500 1250 50",
    "./gen 1000 10000 1000000000 10 bla bla", # this needs to be with A = B = 0
    "./gen 1500 15000 1000000000 50",
    "./gen 2000 20000 1000000000 1000",
    "./gen 2000 20000 1000000000 2500",
    "./gen 2000 20000 1000000000 10000"
]

for test in range(10):
    call("%s > spectacole.in" % COMMANDS[test], shell=True)
    call("./main", shell=True)
    call("cp spectacole.in grader_test%d.in" % (test + 1), shell=True)
    call("cp spectacole.out grader_test%d.ok" % (test + 1), shell=True)
    print("Done with test %d" % (test + 1))
