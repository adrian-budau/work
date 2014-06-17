TESTS = int(input())

for test in range(1, TESTS + 1):
    C, F, X = [float(x) for x in input().split()]

    answer = X / 2
    count = int(X) + 10
    currentCost = 0.0
    currentProduction = 2.0
    for farms in range(count):
        answer = min(answer, currentCost + X / currentProduction)
        currentCost += C / currentProduction
        currentProduction += F

    print("Case #%d: %.7f" % (test, answer))
