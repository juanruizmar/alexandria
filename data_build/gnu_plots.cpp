#include <fstream>
#include <cstdio>

#include "gnu_plots.hpp"
#include "containers.hpp"
#include "histogram.hpp"

void GnuPlot::generate(bool remove_temp_files_) const {
    std::string gnuplot_filename = "__" + uuid_generator().get() + ".gp";
    std::ofstream gnuplot_file(gnuplot_filename);

    tmp_open_files_.insert(gnuplot_filename);

    gnuplot_file << "set title \"" << title_ << "\"\n";
    gnuplot_file << "set xlabel \"" << xlabel_ << "\"\n";
    gnuplot_file << "set ylabel \"" << ylabel_ << "\"\n";
    gnuplot_file << "set output \"" << output_ << "\"\n";
    gnuplot_file << "set terminal png\n";
    gnuplot_file << "unset key\n";

    for (const auto& filename : external_data_files_) gnuplot_file << "plot \"" << filename << "\" with lines" << "\n";
    
    for (const auto& plot : plots_) tmp_open_files_.insert(plot.get().produce_gnuplot_datafile());

    gnuplot_file.close();

    std::string command = "gnuplot " + gnuplot_filename;
    
    std::system(command.c_str());

    if(remove_temp_files_) remove_temp_files();
}
