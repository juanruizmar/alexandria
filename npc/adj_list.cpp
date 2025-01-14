#include "adj_list.hpp"

using namespace std;

#include <iostream>

void adj_list::print(const std::string &path) const{
    std::ofstream file(path);
    file << "p edge " << n_nodes() << " " << n_edges() << std::endl;
    for(auto i: *this) file << "e " << i.first << " " << i.second << std::endl;
}

adj_list::adj_list(const string &path){
    ifstream file(path);
    string line;

    size_t n_edges_;

    getline(file, line);
    sscanf(line.c_str(),"p edge %lu %lu", &n_nodes_, &n_edges_);

    while(getline(file,line)){
        size_t v1, v2;
        sscanf(line.c_str(),"e %lu %lu",&v1,&v2);
        emplace(v1,v2);
    }

    assert(n_edges()==n_edges_);
}
