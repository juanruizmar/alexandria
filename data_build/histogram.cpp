#include "histogram.hpp"
#include "nlohmann/json.hpp"

using namespace std;

void histogram::display(std::ostream &os){
    os << "{\n";
    os << "  \"min\": " << min_ << ",\n";
    os << "  \"max\": " << max_ << ",\n";
    os << "  \"frequencies\": [";
    for(size_t i = 0; true; ++i) {
        os << frequencies_[i];
        if (i==n_segments() - 1) break;
        os << ", ";
    }
    os << "],\n";
    os << "  \"total\": " << total_ << "\n";
    os << "}\n";
}
histogram::histogram(std::ifstream &is) {
    nlohmann::json json_data;
    is >> json_data;

    min_ = json_data["min"];
    max_ = json_data["max"];
    total_ = json_data["total"];

    frequencies_.resize(json_data["frequencies"].size());
    for (size_t i = 0; i < frequencies_.size(); ++i) frequencies_[i] = json_data["frequencies"][i];
}
