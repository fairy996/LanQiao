#include "sys.h"

void LED_Display(unsigned char location,unsigned char enable)
{
    unsigned char temp = 0x00;
    unsigned char temp_old = 0xff;
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