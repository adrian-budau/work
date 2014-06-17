N = int(input())

def gcd(A, B):
    if B == 0:
        return A
    return gcd(B, A % B)

for i in range(1, N + 1):
    for j in range(i + 1, N + 1):
        if gcd(i, j) == i ^ j:
            print(i, j)
