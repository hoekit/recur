// helpers.h
#include <time.h>

enum {                              // Global Flags
    D = 0                           //   1: Show debugging messages
};

int shift(char *str, char *res);

int is_leap_year(int year);         // Returns 1 if leap year. Else 0.

int eom_days(const struct tm* lc);  // Days to end of current month

int mth_days(int year, int month);  // Number of days in given year-month

int next_mday_days(int tgt_mday, const struct tm* lc);
                                    // Days to end of current month
                                    //
int next_next_mday_days(int tgt_mday, const struct tm* lc);
                                    // Days to next, next day of month

