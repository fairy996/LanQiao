#include "key.h"

unsigned char Key_Scan(void)
{

    if(P30 == 0)
        return 7;
    if(P31 == 0)
        return 6;
    if(P32 == 0)
        return 5;
    if(P33 == 0)
        return 4;
    return 0;
}