all: test.out

test.out: test.cpp
	g++ test.cpp -o test.out