CC=g++
FLAGS=-std=c++20

all: tests

tests: task.o tests.o
	$(CC) $(FLAGS) -o tests task.o tests.o -lgtest_main -lgtest -lpthread  

task.o: task.cpp
	$(CC) $(FLAGS) -c task.cpp

tests.o: tests.cpp
	$(CC) $(FLAGS) -c tests.cpp

test: tests
	./tests

clean:
	rm -rf *.o tests