while True:
    N = int(input())
    if N == 0:
        exit()
    X, Y = [int(x) for x in raw_input().split()]

    best = 10**50, 0
    for _ in range(1, N + 1):
        x, y, r = [int(x) for x in raw_input().split()]
        dis = abs(x - X) ** 2 + abs(y - Y) ** 2
        dis = dis ** 0.5
        dis -= r

        best = min(best, (dis, _))

    print(best[1])
