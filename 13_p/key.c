#include "key.h"

sbit L1 = P4 ^ 4;
sbit L2 = P4 ^ 2;
sbit L3 = P3 ^ 5;
sbit L4 = P3 ^ 4;
sbit H1 = P3 ^ 0;
sbit H2 = P3 ^ 1;
sbit H3 = P3 ^ 2;
sbit H4 = P3 ^ 3;

unsigned char key_up = 1;
unsigned char Key_Scan1(void)
{
    if ((key_up == 1) && (H1 == 0 || H2 == 0 || H3 == 0 || H4 == 0))
    {
        Delay_ms(20);
        key_up = 0;
        if (H1 == 0)
            return 1;
        else if (H2 == 0)
            return 2;
        else if (H3 == 0)
            return 3;
        else if (H4 == 0)
            return 4;
        else
            return 0;
    }
    else if (H1 == 1 && H2 == 1 && H3 == 1 && H4 == 1)
    {
        key_up = 1;
        return 0;
    }
    return 0;
}

unsigned char Key_Scan2(void)
{
    unsigned char keynum = 0;
    L1 = L2 = L3 = L4 = 1;
    H1 = H2 = H3 = H4 = 0;
    if (key_up == 1 && (L1 == 0 || L2 == 0 || L3 == 0 || L4 == 0))
    {
        Delay_ms(20);
        if (L1 == 0)
            keynum = 1;
        else if (L2 == 0)
            keynum = 2;
        else if (L3 == 0)
            keynum = 3;
        else if (L4 == 0)
            keynum = 4;
        else
            return 0;
        key_up = 0;
        L1 = L2 = L3 = L4 = 0;
        H1 = H2 = H3 = H4 = 1;
        if (H1 == 0)
            return keynum;
        else if (H2 == 0)
            return keynum + 4;
        else if (H3 == 0)
            return keynum + 8;
        else if (H4 == 0)
            return keynum + 12;
    }
    else if (L1 == 1 && L2 == 1 && L3 == 1 && L4 == 1)
    {
        key_up = 1;
        return 0;
    }
    return 0;
}