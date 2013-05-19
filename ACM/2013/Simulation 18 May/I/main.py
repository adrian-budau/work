N = int(input())

for a1 in range(0, N + 1):
    for a2 in range(0, N + 1 - a1):
        for a3 in range(0, N + 1 - a1 - a2):
            for a4 in range(0, N + 1 - a1 - a2 - a3):
                for a5 in range(0, N + 1 - a1 - a2 - a3 - a4):
                    for a6 in range(0, N + 1 - a1 - a2 - a3 - a4 - a5):
                        for a7 in range(0, N + 1 - a1 - a2 - a3 - a4 - a5 - a6):
                            for a8 in range(0, N + 1 - a1 - a2 - a3 - a4 - a5 - a6 - a7):
                                for a9 in range(0, N + 1 - a1 - a2 - a3 - a4 - a5 - a6 - a7 - a8):
                                    a10 = N - a1 - a2 - a3 - a4 - a5 - a6 - a7 - a8 - a9
                                    B = a9 * '9' + a8 * '8' + a7 * '7' + a6 * '6' + a5 * '5' + a4 * '4' + a3 * '3' + a2 * '2' + a1 * '1' + a10 * '0'
                                    C = B[::-1]

                                    A = "".join(sorted(str(int(B) - int(C))))
                                    
                                    if A == C:
                                        D = int(B[:(N + 1)//2]) + int(B[:(N + 1)//2 - 1:-1])
                                        print(int(B) - int(C), B, D)
