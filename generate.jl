file = open("input.dat", "w")
for i = 1:800
    write(file, rand(Float32, 1024, 1024))
end
close(file)