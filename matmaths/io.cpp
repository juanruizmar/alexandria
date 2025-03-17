#include "io.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

std::string doubleToStringWithScientific(double value, int precision=2) {
    std::ostringstream oss;
    if(1>value || value>=10) oss << std::scientific;
    oss << std::setprecision(precision) << value;

    std::string str = oss.str();
    size_t ePos = str.find('e');
    if(ePos!=std::string::npos && str[ePos+1]=='+') str.erase(ePos+1, 1);
    return str;
}
