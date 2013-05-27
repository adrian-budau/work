class Array
    def biggest
        answer = 0
        for i in 0 .. (self.size - 5)
            now = self[i, 5].map {|x| x.to_i}.compact
            answer = [answer, now.inject(:*)].max
        end
        answer
    end
end

N = File.read('input').split.join.split("")

print N.biggest
