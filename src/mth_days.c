// src/mth_days.c
#include "_private.h"    /* is_leap_year */

// mth_days(2021, 1) == 31          // Month with 31 days
// mth_days(2020,13) == 31          // 2020-13 equivalent to 2021-01
// mth_days(2021, 4) == 30          // Month with 30 days
// mth_days(2000, 2) == 29          // Feb in leap year
// mth_days(2009, 2) == 28          // Feb in non-leap year
int mth_days(int year, int month)   // Days in given year-month
{
    year  = year + (month-1)/12;
    month = (month-1)%12 + 1;       // month > 12 translated to years

    if ((month == 1)                // Months with 31 days
        || (month ==  3)
        || (month ==  5)
        || (month ==  7)
        || (month ==  8)
        || (month == 10)
        || (month == 12))
        return 31;

    else if ((month == 4)           // Months with 30 days
        || (month == 6)
        || (month == 9)
        || (month == 11))
        return 30;

    else if (is_leap_year(year))    // Leap year
        return 29;

    else                            // Non-leap year
        return 28;
}

