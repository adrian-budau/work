while True:
    N = int(raw_input())
    if N == 0:
        break

    V = int((2 * N) ** 0.5)

    while V * (V + 1) // 2 >= N:
        V -= 1

    N -= V * (V + 1) // 2
    V += 1

    if V % 2:
        print V - N + 1, N
    else:
        print N, V - N + 1
