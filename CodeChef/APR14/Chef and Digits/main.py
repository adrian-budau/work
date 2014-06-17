N, M = [int(x) for x in input().split()]
S = input()

answer = [None] * N

count = [0] * 10
for i in range(N):
    c = S[i]
    c = ord(c) - ord('0')

    value = 0
    for j in range(c):
        value += (c - j) * count[j]
    for j in range(c + 1, 10):
        value += (j - c) * count[j]
    count[c] += 1
    answer[i] = value

for _ in range(M):
    x = int(input())
    print(answer[x - 1])


