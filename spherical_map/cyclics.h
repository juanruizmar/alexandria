#ifndef CYCLIC_POINT_250119
#define CYCLIC_POINT_250119

#include <cmath>
#include <cassert>

#include <iostream>
#include <iomanip>

#define Q_WIDTH 2

class cyclic_distance{
    double distance_;

    inline cyclic_distance(const cyclic_distance &d1, const cyclic_distance &d2) noexcept: distance_(sqrt(pow((double)d1,2) + pow((double)d2,2))){}

public:
    inline cyclic_distance(int i) noexcept: cyclic_distance((double)i){}
    explicit inline cyclic_distance(double d) noexcept : distance_(acos(cos(d))){
        assert(0 <= distance_);
        assert(distance_ <= M_PIl);
    }
    
    inline operator double() const noexcept { return distance_; }

    inline friend cyclic_distance operator *(const cyclic_distance &d1, const cyclic_distance &d2) noexcept{
        return cyclic_distance(d1,d2);
    }

    inline friend std::ostream &operator <<(std::ostream &os, const cyclic_distance &D) noexcept{
        return os << std::fixed << std::setprecision(Q_WIDTH) << (double)D;
    }
};

class cyclic_point{};   // The reason for this class was to generalise spheric geometry to an arbitrary large number of dimensions

class sphere_point : public cyclic_point{
    double latt_, long_;

public:
    inline sphere_point(double lattitude, double longitude) noexcept: latt_(lattitude), long_(fmod(longitude+M_PIl,2*M_PIl)-M_PIl){
        assert(-M_PIl/2 <= latt_);
        assert(latt_ <= M_PIl/2);
        assert(-M_PIl <= long_);
        assert(long_ < M_PIl);
    }

    inline double get_latt() const noexcept { return latt_; }
    inline double get_long() const noexcept { return long_; }

    inline friend cyclic_distance operator -(const sphere_point& P1, const sphere_point &P2) noexcept{
        return cyclic_distance(acos(
            sin(P1.get_latt()) * sin(P2.get_latt()) + 
            cos(P1.get_latt()) * cos(P2.get_latt()) * cos(P1.get_long() - P2.get_long())
        ));
    }

    inline friend std::ostream &operator <<(std::ostream &os, const sphere_point &P) noexcept{
        return os << std::fixed << std::setprecision(Q_WIDTH) << P.get_latt() << std::fixed << std::setprecision(Q_WIDTH) << ", " << P.get_long();
    }
};

class year_point : public cyclic_point{
    double month_;

public:
    inline year_point(double month=0) noexcept: month_((month-0.5)/12*2*M_PIl) {
        assert(0 <= month_);
        assert(month_ < 2*M_PIl);
    }

    inline double get_time() const noexcept { return month_; }

    inline friend cyclic_distance operator -(const year_point& P1, const year_point &P2) noexcept{
        return cyclic_distance(acos(cos(P1.get_time() - P2.get_time())));
    }

    inline friend std::ostream &operator <<(std::ostream &os, const year_point &P) noexcept{
        return os << std::fixed << std::setprecision(Q_WIDTH) << P.get_time();
    }
};

#undef Q_WIDTH

#endif