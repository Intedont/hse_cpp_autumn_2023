CC=g++
FLAGS=-std=c++20 -Wunused-variable -Wall -Wextra -Wsign-conversion -fsanitize=address -Werror

all: tests

tests: matrix.o tests.o
	$(CC) $(FLAGS) -o tests matrix.o tests.o -lgtest_main -lgtest -lpthread  

matrix.o: matrix.cpp matrix.hpp
	$(CC) $(FLAGS) -c matrix.cpp

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests