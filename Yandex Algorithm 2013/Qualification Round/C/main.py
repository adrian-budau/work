N = int(input())

ANS = 0
if N % 4 == 0 or N % 4 == 2:
    ANS = 1
    for i in range(0, 32):
        if (2 ** (i + 1)) > N:
            break
        if (2 ** i) & N:
            ANS += 2 ** i
elif N % 4 == 1:
    ANS = (N + 1) // 2

print(N * (N + 1) // 2, ANS)
