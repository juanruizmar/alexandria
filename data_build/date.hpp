#ifndef DATE_HPP
#define DATE_HPP

#include "date.h"

#include <cassert>

class date{
    private:
        unsigned int abs_month_;

    public:
        inline date(unsigned int absolute_month) noexcept: abs_month_(absolute_month){}
        inline date(unsigned int month, unsigned int year) noexcept: date(absolute_month(month, year)){
            assert(1 <= month); assert(month <= 12);
            assert(1901 <= year); assert(year <= 2023);
        }
        inline unsigned int get_year() const { return year_from_abs(abs_month_); }
        inline unsigned int get_month() const { return month_from_abs(abs_month_); }
        inline unsigned int get_abs_month() const { return abs_month_; }

        inline bool operator <(const date &D) const { return get_abs_month() < D.get_abs_month(); }
        inline bool operator ==(const date &D) const { return get_abs_month() == D.get_abs_month(); }
};

#endif
