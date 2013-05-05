M = {}
A = [x for x in open('dictionary.txt')]

count = [0, 1, 2, 3, 4, 5, 7, 11, 14, 19, 25, 33]

longest = 0
total = 0
for x in A:
    y = x[:-5]
    if y not in M:
        M[y] = 0
    M[y] += 1
    longest = max(longest, len(x))
    total += count[len(x)]
answer = 0
for x in M:
    answer = max(answer, M[x])
print(answer)
print(longest)
print(total)
