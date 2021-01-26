// shift-one-char.c
#include <string.h>         /* strlen */

// shift("")    dies with EXIT_FAILURE
// shift("a")   returns ""
// shift("d05") returns "05"
int shift(char *str, char *res)
{
    size_t len   = strlen(str);

    // Return empty string if given empty string
    if (len <= 0) {
        res[0] = '\0';
        return 0;
    }

    // Copy str into res, omitting first char
    int i;
    for (i = 0; i < len-1; i++) {
        res[i] = str[i+1];
    }
    res[i] = '\0';

    return 0;
}

