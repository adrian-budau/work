L = [int(line) for line in open('list.txt')]

T = int(input())
for i in range(1, T + 1):
    A, B = [int(x) for x in input().split(' ')]
    answer = 0
    for X in L:
        if X >= A and X <= B:
            answer += 1
    print("Case #" + str(i) + ": " + str(answer))
