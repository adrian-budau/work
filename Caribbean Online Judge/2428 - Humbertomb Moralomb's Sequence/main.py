from math import ceil

while True:
    N = int(raw_input())
    if N == 0:
        break

    print int(ceil(N ** 0.5))
