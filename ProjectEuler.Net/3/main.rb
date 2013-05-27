N = STDIN.gets.to_i

class Integer
    def factorize
        answer = []
        aux = self
        for i in 1 .. self
            if i * i > aux
                break
            end
            if aux % i == 0
                answer << i
                while aux % i == 0 and i > 1
                    aux /= i
                end
            end
        end
        if aux > 1
            answer << aux
        end
        answer
    end
end

puts N.factorize.max
