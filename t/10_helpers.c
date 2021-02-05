#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "_private.h"
#include "recur.h"

typedef struct {
    char name[10];
} Person;

int num_errors = 0;
int num_tests  = 0;

char tgot[10] = "";
char texp[10] = "";
char tmsg[80] = "";

void is(char *got, char *exp, char *msg) {
    num_tests++;
    if (strcmp(got, exp)) {
        // Differs
        printf(" NOK -- %s\n", msg);
        printf("   got: %s\n", got);
        printf("   exp: %s\n", exp);
        num_errors++;
    } else {
        printf("  OK -- %s\n", msg);
        // Same
    }
}
void check_leap_year(int year, int val)
{
    sprintf(tgot,"%d", is_leap_year(year));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"is_leap_year(%d) == %d",year,val);
    is(tgot,texp,tmsg);
}
void check_eom_days(int yy, int mm, int dd, int val)
{
    struct tm lc[1] = {
        0,
        0,
        0,
        dd,
        mm - 1,
        yy - 1900,
        0,
        0
    };

    sprintf(tgot,"%d", eom_days(lc));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"eom_days(%d,%3d,%3d) == %2d",yy,mm,dd,val);
    is(tgot,texp,tmsg);
}
void check_mth_days(int yy, int mm, int val)
{
    sprintf(tgot,"%d", mth_days(yy,mm));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"mth_days(%d,%2d) == %2d",yy,mm,val);
    is(tgot,texp,tmsg);
}
void check_next_mday_days(int tgt_mday, int yy, int mm, int dd, int val)
{
    struct tm lc[1] = {
        0, 0, 0,
        dd, mm - 1, yy - 1900,
        0, 0 };

    sprintf(tgot,"%d", next_mday_days(tgt_mday, lc));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"next_mday_days(%2d,%5d,%3d,%3d) == %2d",tgt_mday,yy,mm,dd,val);
    is(tgot,texp,tmsg);
}
void check_next_next_mday_days(int tgt_mday, int yy, int mm, int dd, int val)
{
    struct tm lc[1] = {
        0, 0, 0,
        dd, mm - 1, yy - 1900,
        0, 0 };

    sprintf(tgot,"%d", next_next_mday_days(tgt_mday, lc));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"next_next_mday_days(%2d,%5d,%2d,%3d) == %2d",tgt_mday,yy,mm,dd,val);
    is(tgot,texp,tmsg);
}
void check_yday_of(int yy, int mm, int dd, int val)
{
    int year = yy - 1900;       // Convert to struct tm range
    int mon  = mm - 1;
    int mday = dd;

    sprintf(tgot,"%d", yday_of(year,mon,mday));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"ydays_of(%04d,%02d,%02d) == %3d",yy,mm,dd,val);
    is(tgot,texp,tmsg);
}
void check_eoy_days(int yy, int mm, int dd, int val)
{
    struct tm lc[1] = {
        0, 0, 0,
        dd, mm - 1, yy - 1900,
        0, 0, 0 };
    sprintf(tgot,"%d", eoy_days(lc));
    sprintf(texp,"%d", val);
    sprintf(tmsg,"eoy_days(%04d,%02d,%02d) == %3d",yy,mm,dd,val);
    is(tgot,texp,tmsg);
}
void check_recur(char *expr, time_t val)
{
    sprintf(tgot,"%ld", recur(expr));
    sprintf(texp,"%ld", val);
    sprintf(tmsg,"recur(%s) == %ld",expr,val);
    is(tgot,texp,tmsg);
}
void t_status()
{
    if (num_errors)
        printf("\nFound errors: %d of %d\n", num_errors, num_tests);
    else
        printf("\nAll tests passed: %d\n", num_tests);
}

int main(void)
{
    is("ok","ok","is()");
    // exit(EXIT_SUCCESS);

    printf("\n recur()\n");
    check_recur("H10M15y0315",1615778100);
    check_recur("H10M15y0518,y0315", 1615762800);
    check_recur("H10M15y0518y0315",1615778100);
    check_recur("u1", 1612738800);
    check_recur("w1", 1612738800);
    check_recur("u7", 1612652400);
    check_recur("w0", 1612652400);
    // exit(EXIT_SUCCESS);

    printf("\n ydays_of()\n");
    check_yday_of(2020,1,1,0);          // Jan 1 on leap year
    check_yday_of(2021,1,1,0);          // Jan 1 on non-leap year
    check_yday_of(2020,12,31,365);      // Dec 31 on leap year
    check_yday_of(2021,12,31,364);      // Dec 31 on non-leap year
    check_yday_of(2021, 2, 2, 32);      // Feb  2 on non-leap year
    // exit(EXIT_SUCCESS);

    printf("\n eoy_days()\n");
    check_eoy_days(2020,1,1,365);       // Jan 1 on leap year
    check_eoy_days(2021,1,1,364);       // Jan 1 on non-leap year
    check_eoy_days(2020,12,31,0);       // Dec 31 on leap year
    check_eoy_days(2021,12,31,0);       // Dec 31 on non-leap year
    check_eoy_days(2021, 2, 2,332);     // Feb  2 on non-leap year
    // exit(EXIT_SUCCESS);

    printf("\n next_next_mday_days()\n");
    check_next_next_mday_days(30, 2019, 12, 30, 31);
    check_next_next_mday_days(31, 2019, 12, 30, 32);
    check_next_next_mday_days(29, 2019, 12, 30, 61);
    // exit(EXIT_SUCCESS);

    printf("\n next_mday_days()\n");
    check_next_mday_days(30, 2019, 12, 30,  0);
    check_next_mday_days(31, 2019, 12, 30,  1);
    check_next_mday_days(29, 2019, 12, 30, 30);
    check_next_mday_days(31, 2021,  1, 29,  2);
    check_next_mday_days(27, 2021,  1, 29, 29);
    // exit(EXIT_SUCCESS);

    printf("\n mth_days()\n");
    check_mth_days(2021,  1, 31);       // Month with 31 days
    check_mth_days(2020, 13, 31);       // 2020-13 equivalent to 2021-01
    check_mth_days(2021,  4, 30);       // Month with 30 days
    check_mth_days(2000,  2, 29);       // Feb in leap year
    check_mth_days(2009,  2, 28);       // Feb in non-leap year
    check_mth_days(2019, 12, 31);       // Month with 31 days

    printf("\n eom_days()\n");
    check_eom_days(2021, 1, 30,  1);    // Month with 31 days
    check_eom_days(2021, 4,  1, 29);    // Month with 30 days
    check_eom_days(2000, 2, 10, 19);    // Feb in leap year
    check_eom_days(2009, 2, 10, 18);    // Feb in non-leap year

    printf("\n is_leap_year()\n");
    check_leap_year(-100, -1);
    check_leap_year(   0,  1);
    check_leap_year(2021,  0);
    check_leap_year(2020,  1);
    check_leap_year(1800,  0);
    check_leap_year(2000,  1);

    t_status();
    exit(EXIT_SUCCESS);
}

