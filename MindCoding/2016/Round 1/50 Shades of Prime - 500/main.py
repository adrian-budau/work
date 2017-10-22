A = int(input())
B = int(input())

def gcd(A, B):
    if B == 0:
        return A
    return gcd(B, A % B)

answer = -1
for i in range(A, min(A + 10, B) + 1):
    for j in range(max(B - 10, A), B + 1):
        if gcd(i, j) == 1:
            answer = max(answer, j - i + 1)

print(answer)
