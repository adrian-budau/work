from subprocess import check_call

for i in range(1, 11):
    command = "./main < wildfire%02d.in > wildfire%02d.ans" % (i, i)
    print(command)
    check_call(command, shell=True)
    print("Done with test ", i)
