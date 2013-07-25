MODULO = 10**9

def pow10(A, B):
    if B == 0:
        return 1
    if B % 2:
        return (pow10(A, B - 1) * A) % MODULO

    V = pow10(A, B // 2)
    return (V * V) % MODULO

T = int(raw_input())

for _ in xrange(T):
    A, B = [int(x) for x in raw_input().split()]

    print pow10(A, B)

