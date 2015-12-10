from subprocess import call
COMMANDS = [
    "./gen-bad 5 10",
    "./gen-bad 1000 5000",
    "./gen-bad 5000 500000",
    "./gen-bad 10000 500000",
    "./gen-bad 75000 500000",
    "./gen-bad 85000 300000",
    "./gen-good 90000 350000 1 0",
    "./gen-good 100000 500000 5 4",
    "./gen-good 100000 500000 10 9",
    "./gen-good 100000 500000 10 8"
]

for test in range(10):
    call("%s > disconnect.in" % COMMANDS[test], shell=True)
    call("./clean", shell=True)
    call("./transform.py > grader_test%d.in" % (test + 1), shell=True)
    call("cp disconnect.out grader_test%d.ok" % (test + 1), shell=True)
    print("Done with test %d" % (test + 1))
