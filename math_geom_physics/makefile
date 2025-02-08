CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

magnitude.o: magnitude.hpp magnitude.cpp
	$(CXX) $(CXXFLAGS) -c magnitude.cpp

main.out: magnitude.o main.cpp rational.hpp
	$(CXX) $(CXXFLAGS) magnitude.o main.cpp -o main.out
