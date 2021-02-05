// src/next_yday_days.c
// - Functions to support parsing of yexp symbols
#include <stdio.h>      /* printf       */
#include <time.h>       /* struct tm    */
#include "_private.h"

int yday_of(int year,int mon,int mday)
    /* Days from start of year to year/mon/mday
         year: Year since 1900
         mon : Month: 0-11
         mday: Day of month: 1-31
       Valid return values: 0-365
         Jan  1                  = 0
         Dec 31 on leap year     = 365
         Dec 31 on non-leap year = 365 */
{
    struct tm lc[1] = {
        0, 0, 0,
        mday, mon, year,
        0, 0, 0
    };
    time_t epoch = mktime(lc);
    localtime_r(&epoch, lc);
    return lc->tm_yday;
}

int eoy_days(struct tm* lc)
    /* Days from given date in lc to end of year
       Valid return values: 0-365
         Dec 31                  = 0
         Jan  1 on leap year     = 365
         Jan  1 on non-leap year = 364 */
{
    return yday_of(lc->tm_year,11,31)
           - yday_of(lc->tm_year,lc->tm_mon,lc->tm_mday);
}

int next_yday_days(int mm, int dd,
    struct tm* lc)
    /* Days from lc to next mm/dd
         lc is the struct tm for localtime
         If lc on mm dd, return 0.
         Valid values: 0-365
           365 if Feb-29 between lc and mm dd */
{
    D && printf("next_yday_days()\n");
    D && printf("  On lc: %d-%02d-%02d\n",
            lc->tm_year+1900, lc->tm_mon+1, lc->tm_mday);
    D && printf("     mm:%d dd:%d\n", mm, dd);

    int yday0 = yday_of(lc->tm_year, mm-1, dd);
                                        // yday of mmdd in curr year
                                        // Jan 01 = 1
    int yday1 = yday_of(lc->tm_year+1, mm-1, dd);
                                        // yday of mmdd in next year
                                        // Jan 01 = 1

    D && printf("  yday0:%d yday1:%d\n", yday0, yday1);

    if (yday0 >= lc->tm_yday)           // mmdd ON or AFTER lc (curr year)
        return yday0 - lc->tm_yday;     //   difference

   else                                 // mmdd BEFORE lc (next year)
        return eoy_days(lc)             //   add up days to end of year
               + yday1 + 1;             //   and yday of mmdd next year
}

int next_next_yday_days(int mm,
    int dd,struct tm* lc)
    /* Days from lc to next next mm/dd
         lc is struct tm for localtime{
         Valid values: 365-730 */
{
    D && printf("next_next_yday_days()\n");
    D && printf("  On lc: %d-%02d-%2d\n",
            lc->tm_year+1900, lc->tm_mon+1, lc->tm_mday);
    D && printf("     mm:%d dd:%d\n", mm, dd);

    int yday0 = yday_of(lc->tm_year, mm-1, dd);
                                        // yday of mmdd in curr year
    int yday1 = yday_of(lc->tm_year+1, mm-1, dd);
                                        // yday of mmdd in next year
    int yday2 = yday_of(lc->tm_year+2, mm-1, dd);
                                        // yday of mmdd in next,next year
    int year1_days = yday_of(lc->tm_year+1,11,31)+1;
                                        // days in next year

    if (yday0 >= lc->tm_yday)           // mmdd ON or AFTER lc i.e. next year
        return eoy_days(lc)             //   add up days to end of curr year
               + yday1 + 1;             //     + days up to mmdd next year

    else                                // mmdd BEFORE lc i.e. next next year
        return eoy_days(lc)             //   add up days to end of curr year
               + year1_days             //     + days in next year
               + yday2 + 1;             //     + days up to mmdd next next year
}

