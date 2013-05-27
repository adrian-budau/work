class Array
    def prime i
        self[i] == i
    end

    def mark i
        for x in (i + i .. self.size).step i
            self[x] = 0
        end
    end

    def sum
        self.inject(:+)
    end
end

N = STDIN.gets.to_i

a = (0 .. N - 1).to_a

a[1] = 0
for i in 2 .. N - 1
    if a.prime i
        a.mark i
    end
end

puts a.sum
