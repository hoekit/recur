#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

typedef struct {
    char name[10];
} Person;

void is(char *got, char *exp, char *msg) {
    if (strcmp(got, exp)) {
        // Differs
        printf(" NOK -- %s\n", msg);
        printf("   got: %s\n", got);
        printf("   exp: %s\n", exp);
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


int main(void)
{
    is("ok","ok","is()");

    // eom_days()
    check_eom_days(2021, 1, 30,  1);    // Month with 31 days
    check_eom_days(2021, 4,  1, 29);    // Month with 30 days
    check_eom_days(2000, 2, 10, 19);    // Feb in leap year
    check_eom_days(2009, 2, 10, 18);    // Feb in non-leap year

    // is_leap_year()
    check_leap_year(-100, -1);
    check_leap_year(   0,  1);
    check_leap_year(2021,  0);
    check_leap_year(2020,  1);
    check_leap_year(1800,  0);
    check_leap_year(2000,  1);

    exit(EXIT_SUCCESS);
}
