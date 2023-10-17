CC=g++
FLAGS=-std=c++20 -Wunused-variable -Wall -Wextra -Wsign-conversion -fsanitize=address -Werror

all: tests

tests: bigint.o tests.o
	$(CC) $(FLAGS) -o tests bigint.o tests.o -lgtest_main -lgtest -lpthread  

matrix.o: bigint.cpp bigint.hpp
	$(CC) $(FLAGS) -c bigint.cpp

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests