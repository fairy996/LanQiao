#include "sys.h"

void LED_Display(unsigned char location,unsigned char enable)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xff;
    if(enable)
        temp &= ~(0x01 << location);
    else 
        temp |= (0x01 << location);

    if(temp!=temp_old)
    {
        P0 = temp;
        Init_HC138(4);
        temp_old = temp;
    }
}

void Relay(unsigned char flag)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xff;
    if (flag)
        temp |= 0x10;
    else
        temp &= ~0x10;
    if (temp != temp_old)
    {
        P0 = temp;
        P2 = P2 & 0x1f | 0xa0;
        P2 &= 0x1f;
        temp_old = temp;
    }
}