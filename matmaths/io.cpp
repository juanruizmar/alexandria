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

    std::string str = oss.str();
    size_t ePos = str.find('e');
    if(ePos!=std::string::npos && str[ePos+1]=='+') str.erase(ePos+1,1);

    return str;
}
