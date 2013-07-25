T = int(input())

for _ in range(T):
    N, M, K = [int(x) for x in input().split()]

    if N + M <= 3: # we dont have any other cell besides (1, 1) and (N, M)
        print(0)
    elif N == 1 or M == 1:
        print(K)
    else:
        print((K + 1) // 2)
