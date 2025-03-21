#include "magnitude.hpp"

using namespace std;

magnitude magnitude::operator *(const magnitude &other) const{
    magnitude res;
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a+b; });
    return res;
}
magnitude magnitude::operator /(const magnitude &other) const{
    magnitude res;
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a-b; });
    return res;
}

magnitude magnitude::square() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*2; });
    return res;
}
magnitude magnitude::sq_root() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n/2; });
    return res;
}
magnitude magnitude::cube() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*3; });
    return res;
}
magnitude magnitude::inverse() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return -n; });
    return res;
}
magnitude magnitude::pow(rational exp) const{
    magnitude res=*this; 
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [exp](rational n) { return n*exp; });
    return res;
}
