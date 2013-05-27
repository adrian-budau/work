class Integer
    def fact
        return (1 .. self).to_a.inject(:*)
    end
end

class Comb
    @value = 1
    def initialize(n, k)
        @value = n.fact / k.fact / (n - k).fact
    end

    def to_i
        @value
    end
end

N = STDIN.gets.to_i

puts Comb.new(2 * N, N).to_i
