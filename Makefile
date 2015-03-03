all: test.out

test.out: test.cpp Timer.hpp
	g++ -std=c++11 test.cpp -o test.out
