A = raw_input()
B = raw_input()

I = [int(x) for x in A.split(',')[1:]]
A = A.split(',')[0]

A = " ".join(A.split())
B = " ".join(B.split())

N = len(A)
M = len(B)

i = 0
j = 0
BUFFER = ['0'] * 256

def toHex(string):
    result = ''
    for x in string:
        a = hex(ord(x))[2:].upper()
        result += a
    return result

def is_hex(x):
    if ord(x) >= ord('0') and ord(x) <= ord('9'):
        return True

    if ord(x) >= ord('a') and ord(x) <= ord('f'):
        return True

    if ord(x) >= ord('A') and ord(x) <= ord('F'):
        return True

    return False
while i < N:
    if A[i] == ' ':
        if B[j] == ' ':
            i += 1
            j += 1
            continue
        print i, j
        raise Error("Error 1")
    if A[i] != '%' or i == N - 1:
        if B[j] == A[i]:
            i += 1
            j += 1
            continue
        print i,j
        raise Error("Error 2")
    how = A[i + 1]
    i += 2
    where = I[0] * 2
    I = I[1:]
    if how == 'c':
        BUFFER[where:where+2] = toHex(B[j])
        j += 1
        continue

    if how == 's':
        now = ''
        while j < M and B[j] != ' ':
            now += B[j]
            j += 1
        BUFFER[where:where+2*len(now)] = toHex(now)
        continue
    now = ''
    if how == 'u':
        while j < M and ord(B[j]) >= ord('0') and ord(B[j]) <= ord('9'):
            now += B[j]
            j += 1
        now = int(now)
        now = hex(now)[2:].upper()
    else:
        j += 2
        while j < M and is_hex(B[j]):
            now += B[j]
            j += 1
        now = now.upper()
    while len(now) < 8:
        now = '0' + now

    BUFFER[where:where+2] = now[6:8]
    BUFFER[where+2:where+4] = now[4:6]
    BUFFER[where+4:where+6] = now[2:4]
    BUFFER[where+6:where+8] = now[0:2]

print ''.join(BUFFER)
