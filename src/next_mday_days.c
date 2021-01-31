// src/next_mday_days.c
#include <stdio.h>      /* printf       */
#include <time.h>       /* struct tm    */
#include "helpers.h"    /* mth_days     */

int next_mday_days(int tgt_mday, const struct tm* lc)
                                    // Days to end of current month
{
    D && printf("next_mday_days()\n");
    D && printf("  On tm_year:%d tm_mon:%d\n",lc->tm_year,lc->tm_mon);

    int curr_mth_days = mth_days(lc->tm_year+1900, lc->tm_mon+1);
    D && printf("     tgt_mday:%d curr_mth_days:%d curr_mday:%d\n",
        tgt_mday, curr_mth_days, lc->tm_mday);

    int days = (tgt_mday + curr_mth_days - lc->tm_mday) % curr_mth_days;
    D && printf("  Days to next mday is: %d\n", days);
    return days;
}

int next_next_mday_days(int tgt_mday, const struct tm* lc)
                                    // Days to next, next day of month
{
    D && printf("next_next_mday_days()\n");

    int next_days     = next_mday_days(tgt_mday, lc);
    int curr_mth_days = mth_days(lc->tm_year+1900, lc->tm_mon+1);
    int next_mth_days = mth_days(lc->tm_year+1900, lc->tm_mon+2);
    D && printf("  On tgt_mday:%d curr_mday:%d next_days:%d\n",
        tgt_mday, lc->tm_mday, next_days);
    D && printf("     curr_mth_days:%d next_mth_days:%d\n",
        curr_mth_days, next_mth_days);

    if (tgt_mday >= lc->tm_mday)
        return next_days + curr_mth_days;
    else
        return next_days + next_mth_days;
}

