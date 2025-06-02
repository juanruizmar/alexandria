#ifndef GNU_PLOTS_HPP_250601
#define GNU_PLOTS_HPP_250601

#include <string>

#include <set>
#include <vector>

#include "rand_generator.hpp"
#include "uuidentifiable.hpp"

class gnuplot_graphic: public identifiable {
    public:
        virtual std::string produce_gnuplot_datafile() const;

        virtual void print_gnuplot_datafile(std::ostream &os) const = 0;
};

class GnuPlot {
    private:
        std::string title_;
        std::string xlabel_;
        std::string ylabel_;
        std::vector<std::reference_wrapper<const gnuplot_graphic>> plots_;

        mutable std::set<std::string> tmp_open_files_;  // Pendiente: añadir configuración de opciones (color, estilo...)
        std::set<std::string> external_data_files_;

        inline void remove_temp_files() const { for (const auto& filename : tmp_open_files_) std::remove(filename.c_str()); }

    public:
        GnuPlot() = default;
    
        void setTitle(const std::string& title) { title_ = title; }
        void setXLabel(const std::string& xlabel) { xlabel_ = xlabel; }
        void setYLabel(const std::string& ylabel) { ylabel_ = ylabel; }

        inline void addExternalDataFile(const std::string& filename) { external_data_files_.insert(filename); }

        const std::string& getTitle() const { return title_; }
        const std::string& getXLabel() const { return xlabel_; }
        const std::string& getYLabel() const { return ylabel_; }
        
        inline void addPlot(const gnuplot_graphic& plot) { plots_.emplace_back(plot); }

        void generate(std::string filename) const;
};

#endif