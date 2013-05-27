class Integer
    def is_prime
        if self == 1
            return false
        end
        for i in 2 .. self
            if i * i > self
                break
            end
            if self % i == 0
                return false
            end
        end
        true
    end
end

N = STDIN.gets.to_i

found = 0
for i in 2 .. 1000000
    if i.is_prime
        found += 1
    end

    if found == N
        puts i
        break
    end
end
