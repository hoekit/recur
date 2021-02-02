// src/eom_days.c
#include <time.h>       /* struct tm    */
#include "_private.h"   /* is_leap_year */

// eom_days(2021, 1, 30) ==  1      // Month with 31 days
// eom_days(2021, 4,  1) == 29      // Month with 30 days
// eom_days(2000, 2, 10) == 19      // Feb in leap year
// eom_days(2009, 2, 10) == 18      // Feb in non-leap year
int eom_days(const struct tm* lc)   // Days to end of current month
{
    if ((lc->tm_mon == 0)           // Months with 31 days
        || (lc->tm_mon == 2)
        || (lc->tm_mon == 4)
        || (lc->tm_mon == 6)
        || (lc->tm_mon == 7)
        || (lc->tm_mon == 9)
        || (lc->tm_mon == 11))
        return 31 - lc->tm_mday;

    else if ((lc->tm_mon == 3)      // Months with 30 days
        || (lc->tm_mon == 5)
        || (lc->tm_mon == 8)
        || (lc->tm_mon == 10))
        return 30 - lc->tm_mday;

    else if (is_leap_year(lc->tm_year + 1900))  // Feb in a leap year
        return 29 - lc->tm_mday;

    else                                        // Feb in a non-leap year
        return 28 - lc->tm_mday;
}

