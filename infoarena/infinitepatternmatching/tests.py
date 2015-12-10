import subprocess

TESTS = [line for line in open("tests.txt", "r")]

for test in range(1, len(TESTS) + 1):
    print("Generating test %d..." % test, end=" ")
    subprocess.check_call(["./gen"] + TESTS[test - 1].split(), stdout=open("infinitepatternmatching.in", "w"))
    print("done")
    subprocess.check_call(["./main"])
    subprocess.check_call(["cp", "infinitepatternmatching.in", "grader_test%d.in" % test])
    subprocess.check_call(["mv", "infinitepatternmatching.out", "grader_test%d.ok" % test])
