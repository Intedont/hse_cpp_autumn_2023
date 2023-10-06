CC=g++
FLAGS=-std=c++20 -Werror

all: tests

tests: parser.o tests.o
	$(CC) $(FLAGS) -o tests parser.o tests.o -lgtest_main -lgtest -lpthread  

parser.o: parser.cpp
	$(CC) $(FLAGS) -c parser.cpp

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests