#include <vector>
#include "rand_generator.hpp"

#include <iostream>

#include <cassert>

using namespace std;

size_t rand_int_generator::get(size_t max) const{
    assert(max<=generator.max());

    if(max==generator.max()) return get();
    else{
        size_t range = max+1, range_aux = range*(generator.max()/range), aux;
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
