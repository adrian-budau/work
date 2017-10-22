from subprocess import check_call

for i in range(1, 11):
    check_call("./main < orders%02d.in > orders%02d.ans" % (i, i), shell=True)
    print("Done with test ", i)
