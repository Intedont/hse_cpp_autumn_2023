CC=g++
FLAGS=-std=c++20 -Wunused-variable -Wall -Wextra -Wsign-conversion -fsanitize=address -Werror

all: tests

tests: serializer.o tests.o
	$(CC) $(FLAGS) -o tests serializer.o tests.o -lgtest_main -lgtest -lpthread  

serializer.o: serializer.cpp
	$(CC) $(FLAGS) -c serializer.cpp

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests