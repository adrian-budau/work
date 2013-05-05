N = int(input())

for i in range(1, N + 1):
    S = str(i)
    if S != S[::-1]:
        continue

    S = str(i * i)
    if S != S[::-1]:
        continue
    print((i, i * i))
