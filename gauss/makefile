CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

magnitude.o: magnitude.hpp magnitude.cpp
	$(CXX) $(CXXFLAGS) -c magnitude.cpp

constant.o: constant.hpp constant.cpp
	$(CXX) $(CXXFLAGS) -c constant.cpp

main.out: magnitude.o main.cpp rational.hpp constant.o
	$(CXX) $(CXXFLAGS) magnitude.o constant.o main.cpp -o main.out
