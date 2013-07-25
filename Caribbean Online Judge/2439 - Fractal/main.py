T = int(raw_input())

for _ in range(T):
    L, S = [int(x) for x in raw_input().split()]

    answer = -1
    while S >= L:
        answer += 1
        S *= 3
        L *= 5
    print answer
