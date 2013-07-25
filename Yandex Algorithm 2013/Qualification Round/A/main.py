N, L = [int(x) for x in input().split()]

A, B = 0, 0

for _ in range(N):
    T, D = [int(x) for x in input().split()]

    score = 0
    if D == -1:
        score = 1
    elif D <= L:
        score = 2
    else:
        score = 3
    if T == 1:
        A += score
    else:
        B += score

print(str(A) + ":" + str(B))
