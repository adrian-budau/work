def factors(N):
    A = []
    for i in range(2, N + 1):
        if i * i > N:
            break
        if N % i == 0:
            A += [i]
            while N % i == 0:
                N //= i
    if N > 1:
        A += [N]
    return A

def phi(N):
    M = N
    for i in range(2, N + 1):
        if i * i > N:
            break
        if N % i == 0:
            M //= i
            M *= (i - 1)
            while N % i == 0:
                N //= i
    if N > 1:
        M //= N
        M *= (N - 1)
    return M


MOD = 1

def pow2(a, b):
    if b == 0:
        return 1
    if b % 2:
        return (pow2(a, b - 1) * a) % MOD
    x = pow2(a, b // 2)
    return (x * x) % MOD

value = -1
for i in range(100, 1000):
    MOD = 2**23 * i + 1
    PHI = phi(MOD)

    found = False
    for j in range(2, 100):
        if pow2(j, 2**23) == 1:
            print("maybe", MOD, j)
            if pow2(j, 2 ** 22) != 1:
                value = j
                found = True
                break
    if found:
        break

print(MOD, value, phi(MOD))

print(pow2(value, phi(MOD)), pow2(value, 2**22))
