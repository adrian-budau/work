T = int(input())

for _ in range(T):
    P = float(input())
    if P < 0.5:
        P = 1 - P
    answer = 10000 + 10000 * P * (1 - P) * 2 - (1 - P) * 10000
    print(answer)
