#ifndef ASSISTANCE_CONFLICTS__hpp
#define ASSISTANCE_CONFLICTS__hpp

#include <algorithm>
#include <fstream>
#include <random>
#include <string>

#include <cassert>

#include <set>
#include <list>

class adj_list: private std::set<std::pair<std::size_t,std::size_t>>{
    private:
        std::size_t n_nodes_;

    public:
        adj_list(const std::string &path);

        inline std::size_t n_nodes() const { return n_nodes_; }
        inline std::size_t n_edges() const { return size(); }

        inline iterator begin() const { return std::set<std::pair<std::size_t,std::size_t>>::begin(); }
        inline iterator end() const { return std::set<std::pair<std::size_t,std::size_t>>::end(); }

        inline iterator begin() { return std::set<std::pair<std::size_t,std::size_t>>::begin(); }
        inline iterator end() { return std::set<std::pair<std::size_t,std::size_t>>::end(); }

        void print(const std::string &path) const;
};

#endif
