T = int(input())

for test in range(1, T + 1):
    L, N = input().strip().split()

    N = int(N) - 1

    length = 1

    while len(L) ** length <= N:
        N -= len(L) ** length
        length += 1

    answer = ""
    for i in range(length):
        answer = answer + L[N % len(L)]
        N //= len(L)
    print("Case #", test, ": ", answer[::-1], sep="")
