CC=g++
FLAGS=-std=c++20 -Wunused-variable -Wall -Wextra -Wsign-conversion -fsanitize=address -Werror

all: tests

tests: tests.o
	$(CC) $(FLAGS) -o tests tests.o -lgtest_main -lgtest -lpthread  

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests