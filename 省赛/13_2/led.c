#include "led.h"

void Led_control(unsigned char *led)
{
    unsigned char temp,i;
    for(i=0;i<8;i++)
    {
        if(led[i]==1)
        {
            temp &= ~(0x01 << i);
        }
        else
        {
            temp |= (0x01 << i);
        }
    }
    P0 = temp;
    Init_Hc138(4);
}