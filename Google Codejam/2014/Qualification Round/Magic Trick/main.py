TESTS = int(input())
for test in range(1, TESTS + 1):
    print("Case #%d: " % test, end="")

    possible = set(range(1, 17))
    for r in range(2):
        good = int(input())
        for row in range(1, 5):
            A = [int(x) for x in input().split()]
            if row == good:
                possible = possible & set(A)

    if len(possible) == 1:
        print(list(possible)[0])
    elif len(possible) > 1:
        print("Bad magician!")
    else:
        print("Volunteer cheated!")
