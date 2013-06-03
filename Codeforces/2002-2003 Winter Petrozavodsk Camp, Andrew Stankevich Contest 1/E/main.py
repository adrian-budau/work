IN = [line for line in open("nice.in")]
IN = IN[0]

N, M, P = [int(x) for x in IN.split()]
N = N - 1

def mul(A, B):
    X = len(A)
    Y = len(B)
    Z = len(B[0])

    C = [[0] * Z for _ in range(X)]
    for i in range(X):
        for j in range(Y):
            for k in range(Z):
                C[i][k] += A[i][j] * B[j][k]
    for i in range(X):
        for k in range(Z):
            C[i][k] %= P

    return C

BITI = (1 << M)
Z = [[0] * BITI for _ in range(BITI)]

for i in range(BITI):
    A = [0] * M
    for k in range(M):
        if (1 << k) & i:
            A[k] = 1

    for j in range(BITI):
        B = [0] * M
        for k in range(M):
            if (1 << k) & j:
                B[k] = 1

        ok = True
        for k in range(M - 1):
            if A[k] == A[k + 1] and A[k] == B[k] and B[k] == B[k + 1]:
                ok = False
        if ok:
            Z[i][j] += 1

BASE = [[1] * BITI for _ in range(1)]

while N > 0:
    if N % 2:
        BASE = mul(BASE, Z)
    N //= 2
    Z = mul(Z, Z)

out = open("nice.out", "w")
print(sum(BASE[0]) % P, file=out)
