from math import pi

T = int(raw_input())

for _ in range(T):
    N = int(raw_input())

    area = 0.0
    for i in range(N):
        R, H = [int(x) for x in raw_input().split()]
        area += R * R * pi * H

    print "%.2f" % area
