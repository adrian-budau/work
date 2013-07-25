def cmp_to_key(mycmp):
    'Convert a cmp= function into a key= function'
    class K(object):
        def __init__(self, obj, *args):
            self.obj = obj
        def __lt__(self, other):
            return mycmp(self.obj, other.obj) < 0
        def __gt__(self, other):
            return mycmp(self.obj, other.obj) > 0
        def __eq__(self, other):
            return mycmp(self.obj, other.obj) == 0
        def __le__(self, other):
            return mycmp(self.obj, other.obj) <= 0  
        def __ge__(self, other):
            return mycmp(self.obj, other.obj) >= 0
        def __ne__(self, other):
            return mycmp(self.obj, other.obj) != 0
    return K

IN = [line for line in open("circles.in")]

N = int(IN[0])

def dis(x1, y1, x2, y2):
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)

A = []
for i in range(1, N + 1):
    x1, y1, x2, y2, x3, y3 = [int(x) for x in IN[i].split()]

    a = dis(x1, y1, x2, y2)
    b = dis(x1, y1, x3, y3)
    c = dis(x2, y2, x3, y3)

    cos = b + c - a, 4 * b * c
    cos = cos[0] * cos[0], cos[1]

    sin = cos[1] - cos[0], cos[1]

    R = a * sin[1], sin[0]
    A += [R]

def compare(first, second):
    if first[0] * second[1] < first[1] * second[0]:
        return -1
    if first[0] * second[1] > first[1] * second[0]:
        return 1
    return 0

A.sort(key=cmp_to_key(compare))

answer = 0
now = 0
last = (-500, 1)
for x in A:
    if compare(x, last) == 0:
        now += 1
    else:
        now = 1
        last = x
    answer = max(answer, now)

OUT = open("circles.out", "w")
print(answer, file=OUT)
