.PHONY: clean all test
CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Wextra -Iinclude -g

all: bin huffman

bin:
	mkdir -p bin

files:
	mkdir -p test/files	

test: huffman_test files

huffman: bin/main.o bin/huffman.o
	$(CXX) $(CXXFLAGS) bin/main.o bin/huffman.o -o huffman

bin/main.o: src/main.cpp src/huffman.h 
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o bin/main.o

bin/huffman.o: src/huffman.cpp src/huffman.h
	$(CXX) $(CXXFLAGS) -c src/huffman.cpp -o bin/huffman.o

huffman_test: bin bin/test.o bin/huffman_test.o bin/huffman.o bin/autotest.o
	$(CXX) $(CXXFLAGS) bin/test.o bin/huffman_test.o bin/huffman.o bin/autotest.o -o huffman_test

bin/test.o: test/test.cpp test/huffman_test.h test/autotest.h
	$(CXX) $(CXXFLAGS) -c test/test.cpp -o bin/test.o

bin/huffman_test.o: src/huffman.h test/huffman_test.cpp test/huffman_test.h
	$(CXX) $(CXXFLAGS) -c test/huffman_test.cpp -o bin/huffman_test.o

bin/autotest.o: src/huffman.h test/autotest.cpp test/autotest.h test/huffman_test.h
	$(CXX) $(CXXFLAGS) -c test/autotest.cpp -o bin/autotest.o

clean:
	rm -rf bin huffman huffman_test test/files
