from subprocess import call

DATA = [line.strip() for line in open("tests.txt")]

for test in range(1, 11):
    line = DATA[test - 1]

    print("Generating test %d: " % test, end="")

    if call("./gen %s > treespotting.in" % line, shell=True) != 0:
        print("Error generating input")

    if call("./main", shell=True) != 0:
        print("Error running main")

    call("cp treespotting.in grader_test%d.in" % test, shell=True)
    call("cp treespotting.out grader_test%d.ok" % test, shell=True)
    print("OK")
