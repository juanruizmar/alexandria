#ifndef RATIONAL_250208
#define RATIONAL_250208

#include <numeric>
#include <cmath>
#include <stdexcept>
#include <fstream>

class rational{   // A rational number, in exactly 32 bits
    private:
        short n;
        unsigned short d;
        
        inline rational inverse() const {
            if(is_zero()) throw std::invalid_argument("Cannot compute the inverse of zero");
            return rational(d,n);
        }

        inline void reduce() { short div=std::gcd(std::abs(n),d); n/=div, d/=div; }
        inline bool is_zero() const { return n==0; }
        inline bool is_integer() const { return d==1; }
        
    public:
        inline rational(short _n=0, short _d=1): n(_d>0 ? _n : -_n), d(_d>0 ? _d : -_d){
            if(d==0) throw std::invalid_argument("Invalid value for rational. Denominator cannot be zero");
            reduce();
        }
        
        inline explicit operator float() const { return (float)n/d; }

        inline bool operator ==(rational other) { return n*other.d==other.n*d; }
        inline bool operator <(rational other) { return n*other.d<other.n*d; }

        inline rational operator +(rational other) const { return rational(n*other.d+other.n*d,d*other.d); }
        inline rational operator -(rational other) const { return rational(n*other.d-other.n*d,d*other.d); }
        inline rational operator *(rational other) const { return rational(n*other.n,d*other.d); }
        inline rational operator /(rational other) const { if(other.is_zero()) throw std::invalid_argument("Cannot divide by zero"); return rational(n*other.d,d*other.n); }

        inline rational operator +(int other) const { return rational(n+other*d,d); }
        inline rational operator -(int other) const { return rational(n-other*d,d); }
        inline rational operator *(int other) const { return rational(n*other,d); }
        inline rational operator /(int other) const { if(other==0) throw std::invalid_argument("Cannot divide by zero"); return rational(n,d*other); }

        inline rational operator -() { return rational(-n,d); }

        inline friend std::ostream &operator <<(std::ostream &os, rational q){
            if(q.is_zero()) return os << "0";
            else if(q.is_integer()) return os << q.n;
            else return os << q.n << "/" << q.d;
        }
};

inline rational operator +(int z, rational q) { return rational(z)+q; }
inline rational operator -(int z, rational q) { return rational(z)-q; }
inline rational operator *(int z, rational q) { return rational(z)*q; }
inline rational operator /(int z, rational q) { return rational(z)/q; }

#endif
