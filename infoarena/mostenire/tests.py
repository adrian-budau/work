TESTS = ["5 40 100 2 30",
    "20 50 150",
    "50 150 1500",
    "50 150 2000",
    "20 150 3000",
    "20 250 2000",
    "1500 10000 200000 10 2000",
    "1500 20000 200000 50 1000",
    "10000 50000 500000 120 15000",
    "25000 100000 1000000 100 15000", # free
    "25000 100000 1000000 150 10000",
    "30000 100000 1000000 12  10000",
    "35000 100000 1000000", #free
    "40000 100000 1500000",
    "50000 150000 1500000 50 5000",
    "50000 150000 2000000 10 5000", #free
    "700000 2000000000 100000000000000 10 1000000000",
    "800000 1500000000 100000000000000 500000 500000",
    "1000000 2000000000 1000000000000000 1000 150000000",
    "1000000 2000000000 1000000000000000 98000 98000",
]

from subprocess import call

for test in range(20):
    call("./gen %s > mostenire.in" % TESTS[test], shell=True)
    call("./main", shell=True)

    call("cp mostenire.in mostenire.ok", shell=True);
    D = int([line.strip() for line in open("mostenire.out")][0])
    call("echo %d >> mostenire.ok" % D, shell=True)

    call("cp mostenire.in grader_test%d.in" % (test + 1), shell=True)
    call("cp mostenire.ok grader_test%d.ok" % (test + 1), shell=True)

    print("Done with test %d" % (test + 1))
