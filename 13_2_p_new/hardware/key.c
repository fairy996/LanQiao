#include "key.h"
// 定义列引脚
sbit C1 = P4 ^ 4;
sbit C2 = P4 ^ 2;
sbit C3 = P3 ^ 5;
sbit C4 = P3 ^ 4;
// 定义行引脚
sbit L1 = P3 ^ 0;
sbit L2 = P3 ^ 1;
sbit L3 = P3 ^ 2;
sbit L4 = P3 ^ 3;

unsigned char Key_Scan_Matrix()
{
    unsigned char temp;
    C1 = 0;
    C2 = C3 = C4 = 1;
    if (L1 == 0)
        temp = 7;
    else if (L2 == 0)
        temp = 6;
    else if (L3 == 0)
        temp = 5;
    else if (L4 == 0)
        temp = 4;
    C2 = 0;
    C1 = C3 = C4 = 1;
    if (L1 == 0)
        temp = 11;
    else if (L2 == 0)
        temp = 10;
    else if (L3 == 0)
        temp = 9;
    else if (L4 == 0)
        temp = 8;
    C3 = 0;
    C2 = C1 = C4 = 1;
    if (L1 == 0)
        temp = 15;
    else if (L2 == 0)
        temp = 14;
    else if (L3 == 0)
        temp = 13;
    else if (L4 == 0)
        temp = 12;
    C4 = 0;
    C2 = C3 = C1 = 1;
    if (L1 == 0)
        temp = 19;
    else if (L2 == 0)
        temp = 18;
    else if (L3 == 0)
        temp = 17;
    else if (L4 == 0)
        temp = 16;

    P3 = 0xFF;
    return temp;
}

unsigned char Key_Scan_freestanding()
{
    unsigned char temp;
    if(L1 == 0)
        temp = 7;
    else if(L2 == 0)
        temp = 6;
    else if(L3 == 0)
        temp = 5;
    else if(L4 == 0)
        temp = 4;

    P3 = 0xFF;
    return temp;
}