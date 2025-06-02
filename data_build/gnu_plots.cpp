#include <fstream>
#include <cstdio>

#include "gnu_plots.hpp"
#include "containers.hpp"
#include "histogram.hpp"

std::string gnuplot_graphic::produce_gnuplot_datafile() const {
    std::string filename = "__" + get_id() + ".dat";
    std::ofstream file(filename);

    if (!file.is_open()) throw std::runtime_error("Failed to open file for writing: " + filename);

    print_gnuplot_datafile(file);

    file.close();

    return filename;
}

void GnuPlot::generate(std::string filename) const {
    std::string gnuplot_filename = "__" + uuid_generator().get() + ".gp";
    std::ofstream gnuplot_file(gnuplot_filename);

    tmp_open_files_.insert(gnuplot_filename);

    gnuplot_file << "set title \"" << title_ << "\"\n";
    gnuplot_file << "set xlabel \"" << xlabel_ << "\"\n";
    gnuplot_file << "set ylabel \"" << ylabel_ << "\"\n";
    gnuplot_file << "set output \"" << filename << "\"\n";
    gnuplot_file << "set terminal png\n";
    gnuplot_file << "unset key\n";

    for (const auto& filename : external_data_files_) gnuplot_file << "plot \"" << filename << "\" with boxes" << "\n";

    for (const auto& plot : plots_){
        std::string datafile = plot.get().produce_gnuplot_datafile();
        gnuplot_file << "plot \"" << datafile << "\" with boxes" << "\n";
        tmp_open_files_.insert(datafile);
    }

    gnuplot_file.close();

    std::string command = "gnuplot " + gnuplot_filename;
    
    std::system(command.c_str());

    remove_temp_files();
}

int main(){
    list_container<double> values;
    gaussian_generator generator;
    auto it = values.get_forward_modifying_iterator();

    for(int i=0; i<30000; ++i) it.insert_ahead(generator.get());

    histogram hist(values);

    GnuPlot plot;
    plot.setTitle("Histogram of Random Values");
    plot.setXLabel("Value");
    plot.setYLabel("Frequency");
    plot.addPlot(hist);
    plot.generate("histogram_test.png");
}
