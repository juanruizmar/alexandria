#ifndef HISTOGRAM__HPP___
#define HISTOGRAM__HPP___

#include <vector>
#include <list>

#include <limits>
#include <cmath>
#include <fstream>
#include <string>

#include <iostream>

#include "gnu_plots.hpp"

class histogram: public gnuplot_graphic {
    private:
        double min_, max_;
        std::size_t total_;
        std::vector<std::size_t> frequencies_;

    public:
        template<typename container>
        histogram(const container &values): 
        min_(std::numeric_limits<double>::infinity()), 
        max_(-std::numeric_limits<double>::infinity()), 
        total_(values.size()),
        frequencies_(std::vector<std::size_t>(1+std::floor(std::sqrt(values.size())), 0)){
            for(auto i=values.get_forward_reading_iterator(); !i.finished(); i.advance()){
                if(i.payload() < min_) min_=i.payload();
                if(i.payload() > max_) max_=i.payload();
            }

            for(auto i=values.get_forward_reading_iterator(); !i.finished(); i.advance())
                ++frequencies_[std::floor((i.payload() - min_) / segment_length() + 0.5)];
        }
        histogram(std::ifstream &is);

        inline double min() const { return min_; }
        inline double max() const { return max_; }
        
        inline double segment_length() const { return (max_-min_)/(frequencies_.size()-1); }
        inline std::size_t n_segments() const { return frequencies_.size(); }
        inline std::size_t frequency(std::size_t i) const { return frequencies_[i]; }
        inline std::size_t frequency(double x) const { return frequencies_[std::floor((x - min_) / segment_length() + 0.5)]; }
        inline std::size_t total() const { return total_; }

        inline std::pair<double, double> segment(std::size_t i) const {
            return std::pair<double, double>(min_ + (i-0.5) * segment_length(), min_ + (i+0.5) * segment_length());
        }

        void display(std::ostream &os);

        std::string produce_gnuplot_datafile() const;
};

#endif