#ifndef HISTOGRAM__HPP___
#define HISTOGRAM__HPP___

#include <vector>
#include <list>

#include <limits>
#include <cmath>
#include <fstream>
#include <string>

class histogram{
    private:
        double min_, max_;
        std::vector<std::size_t> frequencies_;

    public:
        template<typename container>
        histogram(const container &values): 
        min_(std::numeric_limits<double>::infinity()), 
        max_(-std::numeric_limits<double>::infinity()), 
        frequencies_(std::vector<std::size_t>(std::ceil(std::sqrt(static_cast<double>(values.size()))), 0)){
            for(auto i: values){
                if(i<min_) min_=i;
                if(i>max_) max_=i;
            }

            for(auto i: values) ++frequencies_[std::floor((i-min_)/segment_length())];
        }

        inline double min() const { return min_; }
        inline double max() const { return max_; }
        inline double segment_length() const { return (max_-min_)/n_segments(); }
        inline std::size_t n_segments() const { return frequencies_.size(); }

        inline void print(const std::string &path) { std::ofstream of(path); print(of); }
        void print(std::ostream &os);
};

#endif