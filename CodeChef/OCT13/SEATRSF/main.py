MODULO = 10**9 + 7
def powfast(X, Y):
    if Y == 0:
        return 1
    if Y % 2:
        return (powfast(X, Y - 1) * X) % MODULO
    V = powfast(X, Y // 2)
    return (V * V) % MODULO

T = int(input())

for _ in range(T):
    N, M, K, _ = [int(x) for x in input().split()]

    if N == 1:
        print(0)
        continue
    if M <= K:
        print(0)
        continue

    answer = powfast(K + 1, N) - 2 * powfast(K, N) + powfast(K - 1, N)
    while answer < 0:
        answer += MODULO
    answer = (answer * (M - K)) % MODULO

    print(answer)
