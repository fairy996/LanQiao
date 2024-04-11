#include "led.h"

void LED_Display(unsigned char Pos, unsigned char enable)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xFF;
    if (enable)
        temp &= ~(0x01 << Pos);
    else
        temp |= (0x01 << Pos);
    if (temp != temp_old)
    {
        P0 = temp;
        Init_HC138(4);
        temp_old = temp;
    }
}

void Relay(unsigned char enable)
{
    static unsigned char temp = 0x00;
    static unsigned char temp_old = 0xFF;
    if (enable)
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