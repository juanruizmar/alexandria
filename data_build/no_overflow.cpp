#include "no_overflow.hpp"

#include <iostream>

using namespace std;

const unsigned char n_bits_to_shift = sizeof(uintmax_t)*4;

inline uintmax_t mask_hi() { return ~(((uintmax_t)1<<n_bits_to_shift)-1); }
inline uintmax_t mask_lo() { return (((uintmax_t)1<<n_bits_to_shift)-1); }

inline uintmax_t hi(uintmax_t n) { return n>>n_bits_to_shift; }
inline uintmax_t lo(uintmax_t n) { return n & mask_lo(); }

inline uintmax_t set_lo(uintmax_t &out, uintmax_t n) { assert((out&mask_lo())==0); out |= lo(n); return hi(n); }
inline uintmax_t set_hi(uintmax_t &out, uintmax_t n) { assert((out&mask_hi())==0); out |= (n<<n_bits_to_shift); return hi(n); }

no_overflow::product_computation no_overflow::mul(uintmax_t n1, uintmax_t n2){
    product_computation res;
    uintmax_t carry=0;

    uintmax_t hi1 = hi(n1);
    uintmax_t lo1 = lo(n1);
    uintmax_t hi2 = hi(n2);
    uintmax_t lo2 = lo(n2);

    uintmax_t pll = lo1*lo2;
    uintmax_t phl = hi1*lo2;
    uintmax_t plh = lo1*hi2;
    uintmax_t phh = hi1*hi2;

    carry = set_lo(res.lo_, lo(pll)); assert(carry==0);
    carry = set_hi(res.lo_, hi(pll)+lo(phl)+lo(plh)+carry);
    carry = set_lo(res.hi_, lo(phh)+hi(phl)+hi(plh)+carry);
    carry = set_hi(res.hi_, hi(phh)+carry); assert(carry==0);

    return res;
}
no_overflow::addition_computation no_overflow::add(uintmax_t n1, uintmax_t n2, bool carry){
    addition_computation res;

    res.value_ = n1+n2;
    if(res.value_<n1) res.carry_=true;
    
    if(carry) ++res.value_;
    if(res.value_==0) res.carry_=true;

    return res;
}
