#include <stdio.h>          /* printf */
#include <stdlib.h>         /* EXIT_SUCCESS */
#include "recur.h"

// Usage:
// main y0518 | perl -ae 'print localtime($_)."\n"'

int D = 0;

int main(int argc, char *argv[])
{

    D && printf("argc:%d\n",argc);
    if (argc != 2) exit(EXIT_FAILURE);

    D && printf("recur:%s\n",argv[1]);

    printf("%ld\n",recur(argv[1]));
    exit(EXIT_SUCCESS);
}
