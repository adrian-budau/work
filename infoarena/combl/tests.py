from subprocess import call
DATA = [line.strip().split() for line in open("tests.txt")]
for test in range(1, 11):
    line = DATA[test - 1]
    if call("./%s %s %s %s > grader_test%d.in" % (line[3], line[0], line[1], line[2], test), shell=True) != 0:
        print("Error generating test %d" % test)
        break
    call("cp grader_test%d.in combl.in" % test, shell=True)
    if call("./main", shell=True) != 0:
        print("Error running main on test %d" % test)
        break
    call("cp combl.out grader_test%d.ok" % test, shell=True)
    print("Done generating test %d" % test)
