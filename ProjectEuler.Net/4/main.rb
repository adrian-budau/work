class Integer
    def is_palindrome
        return self.to_s == self.to_s.reverse
    end
end

answer = 1
for i in 100 .. 999
    for j in 100 .. 999
        if (i * j).is_palindrome
            answer = [answer, i * j].max
        end
    end
end

puts answer
