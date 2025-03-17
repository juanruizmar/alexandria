#include "io.hpp"

#include <iomanip>

std::string scientific_double_to_str(double value, int precision){
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << value;
    return oss.str();
}
