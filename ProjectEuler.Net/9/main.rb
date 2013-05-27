N = STDIN.gets.to_i

for i in 1 .. N
    for j in i .. N
        k = N - i - j
        if k <= i or k <= j
            break
        end
        if i * i + j * j == k * k
            puts i, j, k, i * j * k
        end
    end
end
