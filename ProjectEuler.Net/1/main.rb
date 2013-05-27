N = STDIN.gets.to_i

sum = 0
for i in (1 .. N - 1)
    if i % 3 == 0 or i % 5 == 0
        sum += i
    end
end

puts sum
