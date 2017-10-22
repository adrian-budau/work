from subprocess import check_call

MAXK = 10
for i in range(1, 11):
    best = -1

    for mA in range(1, MAXK + 1):
        for mB in range(1, MAXK + 1):
            multA = mA / float(MAXK)
            multB = mB / float(MAXK)
            check_call("./main %.6f %.6f < hospital%02d.in > hospital%02d.out 2> score" % (multA, multB, i, i), shell=True)
            current_score = float([line for line in open("score", "r")][0])
            if current_score > best:
                print("Better with " + str(current_score) + " vs " + str(best))
                best = current_score
                check_call("cp hospital%02d.out hospital%02d.ok" % (i, i), shell=True)
    print("Done with test " + str(i) + " with score " + str(best))
