from datetime import date

T = int(raw_input())

for _ in range(T):
    A, B = raw_input().split()
    A = [int(x) for x in A.split('-')]
    B = [int(x) for x in B.split('-')]

    A = date(A[0], A[1], A[2])
    B = date(B[0], B[1], B[2])

    if B < A:
        A, B = B, A
    print (B - A).days
