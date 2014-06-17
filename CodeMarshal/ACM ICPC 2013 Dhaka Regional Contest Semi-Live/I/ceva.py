N = 20000000

answer = 0
for i in range(1, N + 1):
    if i ^ (i + 1) != 1:
        continue

    x, y = i, i + 1
    while x <= N and y <= N:
        answer += 1
        x *= 2
        y *= 2

print(answer)
