# reading is too slow
IN = [line for line in open("taxi.in")]

M, D, N = [int(x) for x in IN[0].split()]

X = [int(x) for x in IN[1].split()]

Y = []
need = -1

for v in X:
    if v >= M - D and (v < need or need == -1):
        need = v

enough = need

for v in X:
    if v == need:
        need = -1
        continue
    Y += [v]

X = Y
X.sort()

position = 0

OUT = open('taxi.out', 'w')

answer = 0
while position < M:
    if len(X) == 0 and enough == -1:
        print("0", file=OUT)
        exit()
    v = None
    need = M - position
    if position < D:
        need += (D - position)
    
    if len(X) > 0 and enough < need:
        v = X.pop()
    else:
        v = enough
        enough = -1

    if position < D:
        v -= D - position
    else:
        v -= position - D
    if v <= 0:
        continue
    position += v
    answer += 1

print(answer, file=OUT)
