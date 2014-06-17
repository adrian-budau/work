class Move:
    x, y = 0, 0
    direction = 0
    DX = [0, 1, 0, -1]
    DY = [1, 0, -1, 0]

    def __init__(self, x=0, y=0, direction=0):
        self.x = x
        self.y = y
        self.direction = direction

    def __add__(self, other):
        return Move(self.x + other.x, self.y + other.y)
ORDER = int(input("ORDER = "))
STEPS = int(input("STEPS = "))

A = [Move(0, 0)]
B = [Move(0, 0)]

for i in range(1, ORDER + 1):
    pass
