N = int(input())

i = 2
prime = True
while i * i <= N:
    if N % i == 0:
        prime = False
    i += 1

if N == 4:
    print(2)
elif prime:
    print(N - 1)
else:
    print(0)
