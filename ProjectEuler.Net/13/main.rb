puts (File.read('input').split.map {|x| x.to_i}.compact.inject(:+).to_s)[0 .. 9]
