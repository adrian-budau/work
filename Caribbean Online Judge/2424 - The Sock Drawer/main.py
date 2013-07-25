T = int(raw_input())

for test in range(1, T + 1):
    K = int(raw_input())
    C = [int(x) for x in raw_input().split()]

    print "Case " + str(test) + ":", len(C) + 1, max(C) + 1
