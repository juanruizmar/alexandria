#include "io.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

std::string scientific_double_to_str(double value, int precision){
    std::ostringstream oss;
    if(1<=value && value<10) oss << std::fixed;
    else oss << std::scientific;
    oss << std::setprecision(precision) << value;
    return oss.str();
}
