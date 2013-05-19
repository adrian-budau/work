N = [int(x) for x in open("invariant.in")]
N = N[0]

answer = []
if N > 2 and N % 2 == 0:
    aux = "7"
    aux += ((N // 2) - 1) * '6'
    aux += '4'
    aux += (N // 2 - 2) * '3'
    aux += '1'
    answer += [aux]

for a1 in range(0, N // 6 + 1):
    for a3 in range(0, (N - a1 * 6) // 2 + 1):
        for a0 in range(1, N + 1 - a1 * 6 - a3 * 2):
            a9 = N - a1 * 6 - a3 * 2 - a0

            B = a9 * '9' + a1 * '8' + a1 * '7' + a3 * '6' + a1 * '5' + a1 * '4' + a3 * '3' + a1 * '2' + a1 * '1' + a0 * '0'
            C = B[::-1]

            A = "".join(sorted(str(int(B) - int(C))))
            if A == C:
                answer += [B]

for a1 in range(0, N // 2 + 1):
    for a2 in range(0, (N - a1 * 2) // 2 + 1):
        for a3 in range(0, (N - a1 * 2 - a2 * 2) // 2 + 1):
            for a4 in range(0, (N - a1 * 2 - a2 * 2 - a3 * 2) // 2 + 1):
                a9 = N - a1 * 2 - a2 * 2 - a3 * 2 - a4 * 2

                B = a9 * '9' + a1 * '8' + a2 * '7' + a3 * '6' + a4 * '5' + a4 * '4' + a3 * '3' + a2 * '2' + a1 * '1'
                C = B[::-1]

                A = "".join(sorted(str(int(B) - int(C))))
                if A == C:
                    answer += [B]

if N == 1:
    answer = []
out = open("invariant.out", "w")
print(len(answer), file=out)
for i in answer:
    B = i
    C = B[::-1]
    A = str(int(B) - int(C))
    print(A, file=out)
