#include <stdio.h>
#include <stdlib.h>
#include "recur.h"

int main(int argc, char *argv[])
{

    /* Usage:
     *   main y0131     # next recurrence of May 18 in unix epoch */
    if (argc != 2)
        exit(EXIT_FAILURE);

    printf("%ld\n",recur(argv[1]));
    exit(EXIT_SUCCESS);
}
