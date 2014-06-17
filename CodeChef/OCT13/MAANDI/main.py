#!/usr/bin/python
T = int(input())

for _ in range(T):
    N = int(input())
    ANSWER = 0
    for i in range(1, N + 1):
        if i * i > N:
            break
        if N % i == 0:
            nr = str(i)
            if "4" in nr or "7" in nr:
                ANSWER += 1
            nr = str(N // i)
            if "4" in nr or "7" in nr:
                ANSWER += 1
    print(ANSWER)
