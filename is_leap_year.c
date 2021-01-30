// is_leap_year.c

// is_leap_year(2021)  ->  0                // not divisible by 4
// is_leap_year(2020)  ->  1                // not divisible by 100
// is_leap_year(1800)  ->  0                // not divisible by 400
// is_leap_year(2000)  ->  1                // remaining cases
int is_leap_year(int year)          // Returns 1 if leap year. Else 0.
{
    if (year < 0)               return -1;  // invalid input

    if ((year % 4) != 0)        return 0;   // not divisible by 4
    else if ((year % 100) != 0) return 1;   // not divisible by 100
    else if ((year % 400) != 0) return 0;   // not divisible by 400
    else                        return 1;   // remaining cases
}

