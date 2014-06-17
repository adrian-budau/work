N = int(input())

answer = 0
for a in range(1, N):
    for b in range(N):
        ok = True
        for x in range(N):
            if (a * x + b) % N != (a * a * x + a * b + b) % N:
                ok = False
        if ok:
            answer += 1

print(answer)

