#ifndef NO_OVERFLOW_250217
#define NO_OVERFLOW_250217

#include <cstddef>
#include <cstdint>
#include <climits>
#include <utility>

#include <cassert>

namespace no_overflow{
    class addition_computation{
        private:            
            std::uintmax_t value_;
            bool carry_;

        public:
            inline addition_computation(): value_(0), carry_(false) {}

            inline std::uintmax_t value() const { return value_; }
            inline bool carry() const { return carry_; }

            friend addition_computation add(std::uintmax_t n1, std::uintmax_t n2, bool carry);
    };
    class product_computation{
        private:
            std::uintmax_t hi_, lo_;

        public:
            inline product_computation(): hi_(0), lo_(0) {}

            inline std::uintmax_t hi() { return hi_; }
            inline std::uintmax_t lo() { return lo_; }

            friend product_computation mul(std::uintmax_t n1, std::uintmax_t n2);
    };

    addition_computation add(std::uintmax_t n1, std::uintmax_t n2, bool carry);
    product_computation mul(std::uintmax_t n1, std::uintmax_t n2);
};

#endif
