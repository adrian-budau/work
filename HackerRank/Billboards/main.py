N, K = [int(x) for x in input().split()]

DATA = ""
for _ in range(N):
    DATA += input() + " "

A = [int(x) for x in DATA.split()]

from collections import deque

S = [x for x in A]

for i in range(len(A) - 1):
    S[i + 1] += S[i]

D = deque()
D.append((-1, 0))

answer = 0

DP = [0] * len(A)
for i in range(len(A)):
    while D[0][0] < i - K:
        D.popleft()

    DP[i] = S[i] + D[0][1]
    answer = max(answer, DP[i])

    now = None
    if i > 0:
        now = i, DP[i - 1] - S[i]
    else:
        now = i, -S[0]

    while len(D) and D[-1][1] <= now[1]:
        D.pop()
    D.append(now)

print(answer)
