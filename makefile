final: main.o
	g++ -o plagiarismCatcher main.o
main.o: main.cpp HashTable.h
	g++ -std=c++11 -c main.cpp
