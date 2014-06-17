R, N, M = [int(x) for x in input().split()]

V = [1] * N

POS = [0] * 2
for i in range(R):
    D, C, A, B = [int(x) for x in input().split()]
    V[D] = C

    MOVE = 1
    for elem in V[A:B + 1]:
        if elem != "*":
            MOVE *= elem

    POS[i % 2] = (POS[i % 2] + MOVE) % M
    if POS[0] == POS[1]:
        V = [x * POS[0] if isinstance(x, int) and x * POS[0] <= 10**9 else "*" for x in V]

    print(POS[i % 2])


