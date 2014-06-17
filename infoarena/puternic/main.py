MAXDIV = int(input("Max divisors = "))
MAXN = int(input("MAXN = "))

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

dp = [None] * MAXDIV

dp[1] = 1

MAXT = 1 << 84

for i in range(len(primes)):
    nextdp = [None] * MAXDIV

    for j in range(MAXDIV):
        if dp[j] != None:
            now = dp[j]
            for k in range(100):
                index = j * (k + 1)
                if index >= MAXDIV:
                    break
                if now >= MAXT:
                    break
                if nextdp[index] == None:
                    nextdp[index] = now
                else:
                    nextdp[index] = min(nextdp[index], now)
                now *= primes[i]
    dp = nextdp

answer = [1]
for i in range(2, MAXDIV):
    if dp[i] == None:
        continue
    while answer[-1] > dp[i]:
        answer.pop()
    answer.append(dp[i])

answer = answer[:MAXN]
answer = ['"' + str(x) + '"' for x in answer]

print("{" + ", ".join(answer) + "}")
