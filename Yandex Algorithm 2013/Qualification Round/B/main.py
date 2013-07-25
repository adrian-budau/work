MAXV = 10**4 * 8
A = [0] * MAXV

for i in range(2, MAXV):
    if A[i] == 0:
        for j in range(i * 2, MAXV, i):
            A[j] = 1

N = int(input())

for i in range(3, MAXV - 2, 2):
    if A[i] == 0 and A[i + 2] == 0:
        K = i + 1
        if K <= N:
            print(K)
