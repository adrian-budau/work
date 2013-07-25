S = [[0] * 100 for x in range(100)]

for i in range(50):
    S[i][1] = S[i][i] = 1

for i in range(2, 50):
    for j in range(2, i):
        S[i][j] = j * S[i - 1][j] + S[i - 1][j - 1]

fact = [0] * 100
fact[0] = 1
for i in range(1, 50):
    fact[i] = fact[i - 1] * i

A, B = [int(x) for x in input().split()]

if B == 1:
    print("infinity")
    exit()

Z = 1, B - 1

def add_v(first, second):
    return first[0] * second[1] + first[1] * second[0], first[1] * second[1]

ANSWER = 0, 1
for i in range(A + 1):
    ANSWER = add_v(ANSWER, (fact[i] * S[A + 1][i + 1], (B - 1) ** (i + 1)))

def gcd(A, B):
    if B == 0:
        return A
    return gcd(B, A  % B)

v = gcd(ANSWER[0], ANSWER[1])
ANSWER = ANSWER[0] // v, ANSWER[1] // v

print(str(ANSWER[0]) + "/" + str(ANSWER[1]))
