#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(void)
{
    is("ok","ok","is()");
    check_leap_year(-100, -1);
    check_leap_year(   0,  1);
    check_leap_year(2021,  0);
    check_leap_year(2020,  1);
    check_leap_year(1800,  0);
    check_leap_year(2000,  1);

    exit(EXIT_SUCCESS);
}
