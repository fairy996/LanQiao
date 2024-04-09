#include "led.h"

void LED_Display(unsigned char Location, unsigned char enable)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xff;
    if (enable)
        temp &= ~(0x01 << Location);
    else
        temp |= (0x01 << Location);

    if (temp != temp_old)
    {
        P0 = temp;
        Init_HC138(4);
        temp_old = temp;
    }
}

void Beep(unsigned char flag)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xff;
    if (flag)
        temp |= 0x40;
    else
        temp &= ~0x40;
    if (temp != temp_old)
    {
        P0 = temp;
        Init_HC138(5);
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
        Init_HC138(5);
        temp_old = temp;
    }
}