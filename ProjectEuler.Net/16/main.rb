class Integer
    def digits_sum
        self.to_s.split("").map { |x| x.to_i }.compact.inject(:+)
    end
end

N = STDIN.gets.to_i

puts (2 ** N).digits_sum
