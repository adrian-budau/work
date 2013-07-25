T = int(raw_input())

for _ in range(T):
    A, B = [int(x) for x in raw_input().split()]

    if (A + B) % 2:
        print "First"
    else:
        print "Second"
