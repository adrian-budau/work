N = STDIN.gets.to_i

class Integer
    def gcd second
        first = self
        while second != 0
            first, second = second, first % second
        end
        first
    end

    def lcm second
        self * second / gcd(second)
    end
end

class Array
    def lcm
        if self.size == 2
            return self[0].lcm self[1]
        end
        return self[0].lcm self[1 .. -1].lcm
    end
end

puts (1 .. N).to_a.lcm
