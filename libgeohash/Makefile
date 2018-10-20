# Simple Makefile for static library construction and testing
CXX = gcc
CXXFLAGS = -g -std=c++11 
LDFLAGS = -lgtest_main -lstdc++ -lpthread 

all: library test

library:
		$(CXX) $(CXXFLAGS) -c geo_hash.cpp -o geo_hash.o
		ar rcs libgeo_hash.a geo_hash.o
					
test:
		$(CXX) $(CXXFLAGS) -o geo_hash_test geo_hash_test.cpp geo_hash.cpp $(LDFLAGS)
		./geo_hash_test
					
.PHONY: clean
clean:
		rm -f *.o
		rm -f geo_hash_test 
