#include "check_numeric.h"

/* Ensures that the value passed in is numeric */
int isNumeric(const char *str)
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}
