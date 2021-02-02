// _private.h
//   Separate header file declaring all functions that are not exported.
//   This simplifies testing in that testing modules just need to
//   include this header.

int yday_of(int year,int mon,int mday); // Days to year/mon/mday

int eoy_days(struct tm* lc);            // Days to end of year

int next_yday_days(int mm, int dd,
    struct tm* lc);

int next_next_yday_days(int mm,
    int dd,struct tm* lc);

