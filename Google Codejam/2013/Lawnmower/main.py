def read():
    return [int(x) for x in input().split(' ')]

T = int(input())

for test in range(1, T + 1):
    N, M = read()
    A = [read() for i in range(N)]

    ok = True
    while ok == True:
        minim = 500
        where = -1, -1
        for i in range(N):
            for j in range(M):
                if A[i][j] == -1 and A[i][j] < minim:
                    minim = A[i][j]
                    where = i, j

        if minim == 500:
            break

        # check whether it's the column or the row
        x, y = where
        column = True
        row = True
        for i in range(N):
            if A[i][y] != -1 and A[i][y] != minim:
                column = False
                break

        for j in range(M):
            if A[x][j] != -1 and A[x][j] != minim:
                row = False
                break

        if column == False and row == False:
            ok = False
            break

        if row == True:
            for j in range(M):
                A[x][j] = -1
        else:
            for i in range(N):


