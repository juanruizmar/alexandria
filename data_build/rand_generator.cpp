#include <vector>
#include "rand_generator.hpp"

#include <iostream>

#include <cassert>

using namespace std;

size_t rand_int_generator::get(size_t max) const{
    assert(max<=generator.max());

    if(max==generator.max()) return get();
    else{
        size_t range = max+1, aux;
        size_t range_aux = range*(generator.max()/range);
        do aux = get(); while(aux >= range_aux);
        return aux%range;
    }
}

vector<size_t> rand_perm_generator::get(size_t n) const{
    vector<size_t> permutation(n);
    iota(permutation.begin(), permutation.end(), number_from_one_instead_of_zero_ ? 1 : 0);
    shuffle(permutation.begin(), permutation.end(), generator);
    return permutation;
}

bool chance_maker::chance(double prob) const{
    if(prob<0 || prob>1) throw std::invalid_argument("Probability must be a number between 0 and 1 (inclusive).");
    return prob > get();
}

std::vector<size_t> chance_maker::deliver_prizes(size_t n_prizes, std::vector<double> n_tickets){
    std::vector<double> prizes(n_prizes);
    std::vector<size_t> res(n_tickets.size(), 0);
    
    double total_tickets=0;
    for(auto &i: n_tickets){
        total_tickets += i;
        i = total_tickets;
    }
    for(auto &i: n_tickets) i/=total_tickets;

    fill(prizes.begin(), prizes.end());
    std::sort(prizes.begin(), prizes.end());

    size_t next_prize=0;
    for(size_t i=0; i<n_prizes; ++i){
        while(prizes[i]>n_tickets[next_prize]) ++next_prize;
        ++res[next_prize];
    }

    return res;
}
