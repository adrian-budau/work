class Integer
    def square
        return self * self
    end
end

class Array
    def sum_squares
        self.map {|x| x.square }.compact.sum
    end

    def sum
        return inject(:+)
    end
end

N = STDIN.gets.to_i
puts (1 .. N).to_a.sum.square - (1 .. N).to_a.sum_squares
