MAT = File.read("input").split("\n").map {|r| r.split().map {|x| x.to_i}.compact}.compact

N = MAT.size

A = Matrix[
