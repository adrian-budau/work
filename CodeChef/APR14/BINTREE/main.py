N = int(input())

for _ in range(N):
    i, j = [int(x) for x in input().split()]

    i = list(bin(i)[2:])
    j = list(bin(j)[2:])

    while len(i) and len(j) and i[0] == j[0]:
        i.pop(0)
        j.pop(0)

    print(len(i) + len(j))
