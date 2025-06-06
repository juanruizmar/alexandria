#ifndef RAND_GENERATOR___HPP__
#define RAND_GENERATOR___HPP__

#include <random>
#include <algorithm>
#include <limits>

template <typename T> class random_generator{
    protected:
        mutable std::mt19937 generator;

    public:
        inline random_generator(std::size_t seed): generator(seed) {}

        virtual T get() const = 0;

        template <typename iter> inline void fill(const iter &begin, const iter &end) const { for(auto it=begin; it!=end; ++it) *it=get(); }

        virtual ~random_generator() = default;
};

class uniform_generator: public random_generator<double>{
    private:
        mutable std::uniform_real_distribution<double> distribution;

    public:
        inline uniform_generator(): uniform_generator(std::random_device{}()) {}
        inline uniform_generator(std::size_t seed): uniform_generator(0,1,seed) {}
        inline uniform_generator(double min, double max): uniform_generator(min, max, std::random_device{}()) {}
        inline uniform_generator(double min, double max, std::size_t seed): random_generator(seed), distribution(min, max) {}

        inline double get() const { return distribution(generator); }
};
class gaussian_generator: public random_generator<double>{
    private:
        mutable std::normal_distribution<double> distribution;

    public:
        inline gaussian_generator(): gaussian_generator(std::random_device{}()) {}
        inline gaussian_generator(std::size_t seed): gaussian_generator(0,1,seed) {}
        inline gaussian_generator(double mean, double stddev): gaussian_generator(mean, stddev, std::random_device{}()) {}
        inline gaussian_generator(double mean, double stddev, std::size_t seed): random_generator(seed), distribution(mean, stddev) {}

        inline double get() const { return distribution(generator); }
};
class rand_int_generator: public random_generator<std::size_t>{
    private:
        mutable std::uniform_int_distribution<std::size_t> distribution;

    public:
        inline rand_int_generator(std::size_t seed = std::random_device{}()): random_generator<std::size_t>(seed), distribution(0, generator.max()) {}

        inline std::size_t get() const { return distribution(generator); }
        std::size_t get(std::size_t max) const;
};

class chance_maker: private uniform_generator{
    public:
        inline chance_maker(){}
        bool chance(double prob) const;

        // A riffle. It returns a vector with the number of prizes that each participant got.
        // The number of tickets is double instead of int, because it is more general.
        std::vector<std::size_t> deliver_prizes(std::size_t n_prizes, std::vector<double> n_tickets);
};
class rand_perm_generator{
    private:
        mutable std::mt19937 generator;
        bool number_from_one_instead_of_zero_;

    public:
        inline rand_perm_generator(std::size_t seed = std::random_device{}()): generator(seed), number_from_one_instead_of_zero_(false) {}

        std::vector<std::size_t> get(std::size_t n) const;
        inline void set_numbering_from_one(bool number_from_one_instead_of_zero) { number_from_one_instead_of_zero_=number_from_one_instead_of_zero; } 
};

class uuid_generator: private random_generator<std::string>{
    private:
        rand_int_generator generator;

    public:
        uuid_generator(std::size_t seed) = delete;

        inline uuid_generator(): random_generator<std::string>(std::random_device{}()) {}

        std::string get() const;
};

#endif
