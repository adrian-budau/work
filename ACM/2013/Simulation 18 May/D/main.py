D = [int(x) for x in open('divide.in')]
D = D[0]
DORIG = D

POW = 0

while D % 2 == 0:
    POW += 1
    D //= 2
K = D


P = (2 ** 16) // K
if K != 1:
    P += 1

out = open("divide.out", "w")

if K == 1:
    print("y = x >> ", POW, file=out)
    print("return y", file=out)
    exit()



P = (2 ** 32) // DORIG + 1
while True:
    found = False
    for i in range(P // 65536, 65536):
        if P % i == 0:
            if i < 65536 and P // i < 65536:
                P1 = i
                P2 = P // i
                found = True
                break
    if found:
        break
    P += 1

print("a = x *", P1, file=out)
print("a = a >> 16", file=out)
print("a = a *", P2, file=out)
print("a = a >> 16", file=out)
print("c = a *", DORIG, file=out)
print("b = c - x", file=out)
print("b = b +", DORIG - 1, file=out)
print("e = b >> 31", file=out)
print("g = a + e", file=out)
print("return g", file=out)

