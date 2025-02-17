#ifndef DATE_HPP
#define DATE_HPP

#include <cassert>

class date_month_only{
    private:
        unsigned int abs_month;    // Months after january 0 A.D

        static char month_length(unsigned int i) { static const char month_length[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; return month_length[i-1]; }

    public:
        inline date_month_only(unsigned int year, unsigned int month) noexcept: abs_month(year*12 + month-1){
            assert(1 <= month); assert(month <= 12);
        }
        inline unsigned int year() const { return abs_month/12; }
        inline unsigned int month() const { return abs_month%12 + 1; }

        inline bool is_in_a_leap_year() const { return year()%400==0 || (year()%100!=0 && year()%4==0); }

        inline unsigned int n_days() const { return ( month()==2 && is_in_a_leap_year() ) ? 29 : month_length(month()); }

        inline bool operator <(const date_month_only &D) const { return abs_month < D.abs_month; }
        inline bool operator ==(const date_month_only &D) const { return abs_month == D.abs_month; }
};
class date{
    private:
        date_month_only month_;
        unsigned int day_;

    public:
        inline date(unsigned int year, unsigned int month, unsigned int day) noexcept: month_(year, month), day_(day){
            assert(1 <= month); assert(month <= 12);
        }
        inline unsigned int year() const { return month_.year(); }
        inline unsigned int month() const { return month_.month(); }
        inline unsigned int day() const { return day_; }

        inline bool is_in_a_leap_year() const { return month_.is_in_a_leap_year(); }

        inline bool operator <(const date &D) const { return month_<D.month_ || (month_==D.month_ && day_<D.day_); }
        inline bool operator ==(const date &D) const { return month_==D.month_ && day_==D.day_; }
};

class clock_time{
    private:
        unsigned int abs_seconds;   // Seconds after midnight

        inline clock_time(unsigned int absolute_seconds) noexcept: abs_seconds(absolute_seconds){}

    public:
        inline clock_time(unsigned int time, unsigned int minute, unsigned int second) noexcept: abs_seconds(time*3600 + minute*60 + second){
            assert(time <= 23);
            assert(minute <= 59);
            assert(second <= 59);
        }
        inline unsigned int time() const { return abs_seconds / 3600; }
        inline unsigned int minute() const { return (abs_seconds % 3600) / 60; }
        inline unsigned int second() const { return abs_seconds % 60; }

        inline bool operator <(const clock_time &T) const { return abs_seconds < T.abs_seconds; }
        inline bool operator ==(const clock_time &T) const { return abs_seconds == T.abs_seconds; }
};
class date_time{
    private:
        date D;
        clock_time T;

    public:
        inline date_time(unsigned int y, unsigned int m, unsigned int d, unsigned int t, unsigned int min): date_time(y, m, d, t, min, 0) {}
        inline date_time(unsigned int y, unsigned int m, unsigned int d, unsigned int t, unsigned int min, unsigned int s): D(y,m,d), T(t,min,s) {}

        inline unsigned int year() const { return D.year(); }
        inline unsigned int month() const { return D.month(); }
        inline unsigned int day() const { return D.day(); }
        inline unsigned int time() const { return T.time(); }
        inline unsigned int minute() const { return T.minute(); }
        inline unsigned int second() const { return T.second(); }

        inline bool operator <(const date_time &DT) const { return D < DT.D || (D==DT.D && T<DT.T); }
        inline bool operator ==(const date_time &DT) const { return D == DT.D && T==DT.T; }
};

#endif
