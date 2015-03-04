all: test.out uv.out

test.out: test.cpp Timer.hpp
	g++ -std=c++11 test.cpp -o test.out

uv.out: uv.cpp Timer.hpp
	g++ -std=c++11 uv.cpp -luv -o uv.out