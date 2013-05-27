class Integer
    @@chain_length = {1 => 0}
    def next
        if self % 2 == 0
            return self / 2
        end
        self * 3 + 1
    end

    def chain_length
        if @@chain_length[self] != nil
            return @@chain_length[self]
        end
        @@chain_length[self] = 1 + self.next.chain_length
    end
end

class Pair
    include Comparable
    def initialize(first, second)
        @first = first
        @second = second
    end

    def first
        @first
    end

    def second
        @second
    end

    def <=>(y)
        @first <=> y.first
    end

    def to_s
        "(" + @first.to_s + ", " + @second.to_s + ")"
    end
end

N = STDIN.gets.to_i

answer = Pair.new(0, 1)
for i in 1 .. N
    answer = [answer, Pair.new(i.chain_length, i)].max
end

puts answer
