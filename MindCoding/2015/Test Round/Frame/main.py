while True:
    try:
        X, Y = [int(x) for x in raw_input().split()]
        N = int(raw_input())
        for _ in range(N):
            A = int(input())
            if A == 1:
                print("YES")
            elif A == 2:
                print("YES")
            else:
                ok = False
                if X % A == 0 and Y % A == 2:
                    ok = True
                if X % A == 1 and Y % A == 1:
                    ok = True
                if X % A == 2 and Y % A == 0:
                    ok = True
                if ok:
                    print("YES")
                else:
                    print("NO")
    except:
        break

