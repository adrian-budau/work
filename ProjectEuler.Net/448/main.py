def many(N):
    if N == 1:
        return 1
    if N % 2 == 1:
        return many(N - 1) + N * (N + 1) // 2

    K = N // 2
    X = many(K)
    return 2 * X + K * K * (K + 1)

N = int(input())
A = {}
B = {}

def sumrange(x, y):
    return y * (y + 1) // 2-  x * (x - 1) // 2

A[1] = 1
B[1] = 1

at = 0
def answerunique(N):
    if N in B:
        return B[N]

    global at
    at += 1
    if at % 10 == 0:
        print("now at", at)
    now = many(N)
    i = 2
    while i * i <= N:
        now -= answerunique(N // i) * i
        i += 1

    last = i - 1
    total = N // i
    while total > 0:
        y = N // total
        x = (N // (total + 1)) + 1
        x = max(x, last + 1)

        now -= sumrange(x, y) * answerunique(total)

        total -= 1
        last = y

    B[N] = now
    return now

def answer(N):
    if N in A:
        return A[N]

    now = many(N)
    i = 2
    while i * i <= N:
        now -= answerunique(N // i) * (i - 1)
        i += 1

    last = i - 1
    total = N // i
    while total > 0:
        # N / x = total, x minimum
        # N / y = total, y maximum
        # N / total = y + some remainder
        # becuase (y + 1) * total > N
        y = N // total
        x = (N // (total + 1)) + 1
        x = max(x, last + 1)

        now -= sumrange(x - 1, y - 1) * answerunique(total)

        total -= 1
        last = y
    A[N] = now
    return now

print(answerunique(N))
print(answer(N))
print(answer(N) % 999999017)
