def good(S):
    return sum([int(x) * int(x) for x in S]) < 10

A = []
def back(S, total):
    if len(S) > 30:
        return

    global A
    if len(S):
        if good(S + S[::-1]):
            A += [int(S + S[::-1])]
        if good(S[:-1] + S[::-1]):
            A += [int(S[:-1] + S[::-1])]

    if total >= 10:
        return

    for i in range(4):
        if len(S) == 0 and i == 0:
            continue
        back(S + str(i), total + i * i + i * i)

back("", 0)
A.sort()

out = open('list.txt', 'w')
for X in A:
    print(X * X, file=out)
