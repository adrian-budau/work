class Integer
    def triangle
        self * (self + 1) / 2
    end

    def divisors
        divs = []
        for i in 1 .. self
            if i * i > self
                break
            end
            if self % i == 0
                divs << i
                if self / i != i
                    divs << self / i
                end
            end
        end
        divs
    end
end

N  = STDIN.gets.to_i

answer = 1
while answer.triangle.divisors.size < N
    answer += 1
end

puts answer, answer.triangle
