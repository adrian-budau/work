TESTS = [
    (10, 1, 2), # part of the 30 points
    (100, 1, 100), # this one too
    (1500, 1, 3000),
    (50000, 1, 4500),
    (100000, 1, 4500), # that's it, 50 points
    (100, 50, 100),
    (5000, 1000, 3000),
    (10000, 100, 45000),
    (100000, 100000, 45000),
    (100000, 10000, 45000)
]
from subprocess import call

for test in range(1, len(TESTS) + 1):
    N, CYCLE, MODULO = TESTS[test - 1]

    if call("./generator " + str(N) + " " + str(CYCLE) + " " + str(MODULO) + " > sistem3.in", shell=True) != 0:
        print("Problem generating test", test)
        break

    if call("./main", shell=True) != 0:
        print("Problem running test", test)
        break

    if call("./grader_eval > result 2> /dev/null", shell=True) != 0:
        print("Problim verifying test", test)
        break

    points = int([line for line in open("result")][0])
    if points != 10:
        print("Wrong answer on test", test)
        break

    call("mv sistem3.in grader_test" + str(test) + ".in", shell=True)
    print("Done test", test)
