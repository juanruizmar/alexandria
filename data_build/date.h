#ifndef DATE_H
#define DATE_H

inline unsigned int absolute_month(unsigned int month, unsigned int year) { return year*12 + month -1; }
inline unsigned int month_from_abs(unsigned int absolute_month) { return absolute_month % 12 +1; }
inline unsigned int year_from_abs(unsigned int absolute_month) { return absolute_month/12 ;}

#endif
