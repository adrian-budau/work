A = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

B = [1] * 51

for x in A:
    B[x] = x

for i in range(1, 51):
    B[i] *= B[i - 1]

while True:
    N = int(raw_input())
    if N == 0:
        break
    print B[N]
