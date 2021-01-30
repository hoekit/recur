#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

typedef struct {
    char name[10];
} Person;

int num_errors = 0;
int num_tests  = 0;

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
    char got[10] = "";
    char exp[10] = "";
    char msg[80] = "";
    sprintf(got,"%d", is_leap_year(year));
    sprintf(exp,"%d", val);
    sprintf(msg,"is_leap_year(%d) == %d",year,val);
    is(got,exp,msg);
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

    char got[10] = "";
    char exp[10] = "";
    char msg[80] = "";
    sprintf(got,"%d", eom_days(lc));
    sprintf(exp,"%d", val);
    sprintf(msg,"eom_days(%d,%3d,%3d) == %2d",yy,mm,dd,val);
    is(got,exp,msg);
}
void check_mth_days(int yy, int mm, int val)
{
    char got[10] = "";
    char exp[10] = "";
    char msg[80] = "";
    sprintf(got,"%d", mth_days(yy,mm));
    sprintf(exp,"%d", val);
    sprintf(msg,"mth_days(%d,%2d) == %2d",yy,mm,val);
    is(got,exp,msg);
}
void check_next_mday_days(int tgt_mday, int yy, int mm, int dd, int val)
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

    char got[10] = "";
    char exp[10] = "";
    char msg[80] = "";
    sprintf(got,"%d", next_mday_days(tgt_mday, lc));
    sprintf(exp,"%d", val);
    sprintf(msg,"next_mday_days(%2d,%5d,%3d,%3d) == %2d",tgt_mday,yy,mm,dd,val);
    is(got,exp,msg);
}
void check_next_next_mday_days(int tgt_mday, int yy, int mm, int dd, int val)
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

    char got[10] = "";
    char exp[10] = "";
    char msg[80] = "";
    sprintf(got,"%d", next_next_mday_days(tgt_mday, lc));
    sprintf(exp,"%d", val);
    sprintf(msg,"next_next_mday_days(%2d,%5d,%2d,%3d) == %2d",tgt_mday,yy,mm,dd,val);
    is(got,exp,msg);
}
void status()
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

    status();
    exit(EXIT_SUCCESS);
}
