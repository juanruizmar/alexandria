CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -DNDEBUG
DEBUGFLAGS = -O0 -g -DDEBUG_MODE

MODE = RELEASE
ifeq ($(MODE),DEBUG)
    CXXFLAGS += $(DEBUGFLAGS)
else
    CXXFLAGS += $(OPTFLAGS)
endif

adj_list.o: adj_list.cpp adj_list.hpp
	$(CXX) $(CXXFLAGS) -c adj_list.cpp

graph.o: graph.cpp graph.hpp
	$(CXX) $(CXXFLAGS) -c graph.cpp

CNF_SAT.o: CNF_SAT.cpp CNF_SAT.hpp
	$(CXX) $(CXXFLAGS) -c CNF_SAT.cpp

clique_to_cnf_translator.out: clique_to_cnf_translator.cpp CNF_SAT.o graph.o adj_list.o
	$(CXX) $(CXXFLAGS) CNF_SAT.o graph.o adj_list.o clique_to_cnf_translator.cpp -o clique_to_cnf_translator.out

decision.out: decision.cpp CNF_SAT.o graph.o adj_list.o
	$(CXX) $(CXXFLAGS) CNF_SAT.o graph.o adj_list.o decision.cpp -o decision.out

search.out: search.cpp CNF_SAT.o graph.o adj_list.o
	$(CXX) $(CXXFLAGS) CNF_SAT.o graph.o adj_list.o search.cpp -o search.out

optimisation.out: optimisation.cpp CNF_SAT.o graph.o adj_list.o
	$(CXX) $(CXXFLAGS) CNF_SAT.o graph.o adj_list.o optimisation.cpp -o optimisation.out

main.out: graph.o CNF_SAT.o adj_list.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp CNF_SAT.o graph.o adj_list.o -o main.out
