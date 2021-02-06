// _private.h
//   Separate header file declaring all functions that are not exported.
//   This simplifies testing in that testing modules just need to
//   include this header.

#include <time.h>

#ifndef D
#define D 0
#endif


int shift(char *str, char *res);

int is_leap_year(int year);         // Returns 1 if leap year. Else 0.

int eom_days(const struct tm* lc);  // Days to end of current month

int mth_days(int year, int month);  // Number of days in given year-month

int next_mday_days(int tgt_mday, const struct tm* lc);
                                    // Days to end of current month

int next_next_mday_days(int tgt_mday, const struct tm* lc);
                                    // Days to next, next day of month

int yday_of(int year,int mon,int mday); // Days to year/mon/mday

int eoy_days(struct tm* lc);            // Days to end of year

int next_yday_days(int mm, int dd, struct tm* lc);

int next_next_yday_days(int mm, int dd,struct tm* lc);

