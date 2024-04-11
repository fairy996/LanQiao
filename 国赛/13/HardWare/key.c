#include "key.h"

sbit L4 = P3 ^ 3;
sbit L3 = P3 ^ 2;
sbit L2 = P3 ^ 1;
sbit L1 = P3 ^ 0;

unsigned char Key_Scan()
{
    unsigned char temp;
    if (L1 == 0)
        temp = 7;
    else if (L2 == 0)
        temp = 6;
    else if (L3 == 0)
        temp = 5;
    else if (L4 == 0)
        temp = 4;

    P3 = 0xFF;
    return temp;
}