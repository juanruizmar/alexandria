def duplicate_single_month(month, year):
    return month, year, month, year

def months_range_as_absolute_months(start_month, start_year, end_month, end_year):
    absolute_start_month = start_year*12 + start_month - 1
    absolute_end_month = end_year*12 + end_month

    return range(absolute_start_month, absolute_end_month)

def months_range_as_strings(start_month, start_year, end_month, end_year):
    strings = []

    for month in months_range_as_absolute_months(start_month, start_year, end_month, end_year):
        strings.append(str(month//12) + '_' + str(month%12+1).zfill(2))

    return strings

def months_range_as_pairs(start_month, start_year, end_month, end_year):
    pairs = []

    for month in months_range_as_absolute_months(start_month, start_year, end_month, end_year):
        pairs.append([month%12+1,month//12])

    return pairs