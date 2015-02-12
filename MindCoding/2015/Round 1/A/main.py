T = int(raw_input())

good = set()
for i in range(24):
    h = str(i)
    if len(h) == 1:
        h = "0" + h
    good.add(h + ":00")
    good.add(h + ":" + h)
    if (int(h[::-1]) < 60):
        good.add(h + ":" + h[::-1])
good.add("01:23")
good.add("12:34")
good.add("23:45")
good.add("10:24")
good.add("20:48")

for _ in range(T):
    S = raw_input()
    if S in good:
        print("YES")
    else:
        print("NO")
