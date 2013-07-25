test = 0
while True:
    C, Q, K = [int(x) for x in raw_input().split()]

    if C == 0 and Q == 0 and K == 0:
        break

    test += 1
    print "Case #" + str(test) + ":"

    V = [0] * C
    for i in xrange(C):
        S, V[i] = raw_input().split()
        V[i] = int(V[i])

    P = []
    for i in xrange(C):
        for j in xrange(V[i]):
            P += [i + 1]

    for _ in xrange(Q):
        answer = []
        A = [int(x) - 1 for x in raw_input().split()]

        used = [False] * len(P)
        used[A[-7]] = True

        for x in A[-K:]:
            used[x] = True

        for x in xrange(len(P)):
            if used[x]:
                continue
            if P[x] == P[A[-1]]:
                continue
            answer += [x]

        if len(answer):
            print " ".join([str(x + 1) for x in answer])
        else:
            print "Buy other blouse"

    print ""
