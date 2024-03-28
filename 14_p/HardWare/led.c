#include "led.h"


void led_control(unsigned char *led)
{
    uchar temp;
    uchar i = 0;
    for(i = 0; i < 8; i++)
    {
        if(led[i] == 1)
        {
            temp &= ~(0x01 << i);
        }
        else
        {
            temp |= (0x01 << i);
        }
    }
    P0 = temp;
    InitHC138(4);
}