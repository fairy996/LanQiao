#include "led.h"

void Led_Display(unsigned char pos, unsigned char enable)
{
    static uchar temp = 0x00;
    static uchar temp_old = 0xff;
    if (enable)
        temp |= 0x01 << pos;
    else
        temp &= ~(0x01 << pos);

    if (temp != temp_old)
    {
        P0 = ~temp;
        InitHC138(4);
        temp_old = temp;
    }
}