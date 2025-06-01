#ifndef GNU_PLOTS_HPP_250601
#define GNU_PLOTS_HPP_250601

#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <cstdio>
#include <iostream>
#include <fstream>

#include <set>
#include <vector>

#include "rand_generator.hpp"
#include "uuidentifiable.hpp"

class gnuplot_graphic: public identifiable {
    public:
        virtual std::string produce_gnuplot_datafile() const = 0;
};

class GnuPlot {
    private:
        class plot{ // A eliminar
            private:
                std::vector<double> x_data_;
                std::vector<double> y_data_;

                std::string title_;
                
            public:
                plot(const std::vector<double>& x_data, const std::vector<double>& y_data): x_data_(x_data), y_data_(y_data) {}

                const std::vector<double>& getXData() const { return x_data_; }
                const std::vector<double>& getYData() const { return y_data_; }

                void setTitle(const std::string& title) { title_ = title; }
                const std::string& getTitle() const { return title_; }

                std::string write() const {
                    std::string gnuplot_filename = uuid_generator().get();

                    std::ofstream gnuplot_file("__" + gnuplot_filename + ".dat");

                    for (size_t i = 0; i < x_data_.size(); ++i) {
                        gnuplot_file << x_data_[i] << " " << y_data_[i] << "\n";
                    }

                    return "__" + gnuplot_filename + ".dat";
                }
        };

        std::string title_;
        std::string xlabel_;
        std::string ylabel_;
        std::string output_;
        std::vector<std::reference_wrapper<const gnuplot_graphic>> plots_;

        mutable std::set<std::string> tmp_open_files_;
        std::set<std::string> external_data_files_;

        inline void remove_temp_files() const { for (const auto& filename : tmp_open_files_) std::remove(filename.c_str()); }

    public:
        GnuPlot() = default;
    
        void setTitle(const std::string& title) { title_ = title; }
        void setXLabel(const std::string& xlabel) { xlabel_ = xlabel; }
        void setYLabel(const std::string& ylabel) { ylabel_ = ylabel; }
        void setOutput(const std::string& filename) { output_ = filename; }

        inline void addExternalDataFile(const std::string& filename) { external_data_files_.insert(filename); }

        const std::string& getOutput() const { return output_; }
        const std::string& getTitle() const { return title_; }
        const std::string& getXLabel() const { return xlabel_; }
        const std::string& getYLabel() const { return ylabel_; }
        
        inline void addPlot(const gnuplot_graphic& plot) { plots_.emplace_back(plot); }

        void generate(bool remove_temp_files = true) const;
};

#endif