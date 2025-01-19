#ifndef EPSG_3857_H
#define EPSG_3857_H

#include <cmath>
#include <utility>

template<typename T>
inline std::pair<T,T> epsg3857_to_radians(std::pair<T,T> epsg3857_coords){
    const double R = 6378137;
    return std::pair<T, T> (atan(sinh(epsg3857_coords.second/R)), epsg3857_coords.first/R);
}

#endif