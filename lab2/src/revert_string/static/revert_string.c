#include "revert_string.h"

void RevertString(char *str)
{
	int len = strlen(str);
    char *p = str;
    char *q = str + len - 1;
    while (p < q)
    {
        char tmp = *p;
        *p = *q;
        *q = tmp;
 
        p++;
        q--;
    }
}

