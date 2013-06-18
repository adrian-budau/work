T = int(input())

from collections import Counter

for _ in range(T):
    S = input()
    A, B = None, None
    A, B = S[:len(S) // 2], S[-(len(S) // 2):]

    a = Counter(A)
    b = Counter(B)

    if a == b:
        print("YES")
    else:
        print("NO")
