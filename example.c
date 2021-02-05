#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "recur.h"

/* Example using recur(char *).
 * Compare current date and recur output:
 *   date && main H13M00u5 */
int main(int argc, char *argv[])
{

    /* Usage:
     *   main y0131     # next recurrence of May 18 in unix epoch */
    if (argc != 2)
        exit(EXIT_FAILURE);

    time_t next = recur(argv[1]);
    printf("%ld\n", next);

    /* Comment out line below to return only the epoch */
    printf("%s\n", ctime(&next));

    exit(EXIT_SUCCESS);
}
