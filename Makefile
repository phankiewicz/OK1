all: main.o
	g++ -o OK1 main.o
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
clean:
	rm OK1 *.o
love:
	@echo "not war?"
