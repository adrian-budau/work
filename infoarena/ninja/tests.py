TESTS = [
    (5, 5, 5),
    (300, 300, 300),
    (2000, 2000, 2000),
    (20000, 20000, 20000),
    (30000, 30000, 30000),
    (40000, 40000, 40000),
    (50000, 50000, 50000),
    (60000, 60000, 60000),
    (100000, 100000, 100000),
    (100000, 100000, 100000)
]

from subprocess import call

for index, test in enumerate(TESTS):
    N, M, K = test
    call("./generator " + str(N) + " " + str(M) + " " + str(K) + " > ninja.in", shell=True)
    call("./main")

    call("cp ninja.in grader_test" + str(index + 1) + ".in", shell=True)
    call("cp ninja.out grader_test" + str(index + 1) + ".ok", shell=True)

    print("Done test ", index + 1)
