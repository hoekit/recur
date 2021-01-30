// src/next_mday_days.c
#include <stdio.h>      /* printf       */
#include <time.h>       /* struct tm    */
#include "helpers.h"    /* mth_days     */

int D = 0;

int next_mday_days(int tgt_mday, const struct tm* lc)
                                    // Days to end of current month
{
    D && printf("next_mday_days()\n");
    int curr_mth_days = mth_days(lc->tm_year+1900, lc->tm_mon+1);
    D && printf("  Given tgt_mday:%d curr_mth_days:%d curr_mday:%d\n",
        tgt_mday, curr_mth_days, lc->tm_mday);
    int days = (tgt_mday + curr_mth_days - lc->tm_mday) % curr_mth_days;
    D && printf("  Days to next mday is: %d\n", days);
    return days;
}

