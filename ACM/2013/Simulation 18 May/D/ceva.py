def tryd(x):
    a = x * 34179
    a = a >> 16
    a = a * 41887
    a = a >> 16
    c = a * 3
    b = c - x
    b = b + 2
    if b < 0:
        b += 2 ** 32
    e = b >> 31
    g = a + e
    return g

for i in range(65536):
    if i // 3 != tryd(i):
        print(i, i // 3, tryd(i))
        break
