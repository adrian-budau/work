def gcd(A, B):
    if B == 0:
        return A
    return gcd(B, A % B)

N = [int(line) for line in open("china.in")]
N = N[0]

out = open("china.out", "w")
for i in range(N // 2, 0, -1):
    if  gcd(N, i) == 1:
        print(i, file=out)
        exit()
