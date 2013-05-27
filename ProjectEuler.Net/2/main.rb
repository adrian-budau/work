class Array
    def next
        return [self[1], self[0] + self[1]]
    end
end

class Integer
    def even
        return self % 2 == 0
    end
end

MAXN = STDIN.gets.to_i

fib = [1, 2]
sum = 0
while fib[1] < MAXN
    if fib[1].even
        sum += fib[1]
    end
    fib = fib.next
end

puts sum
