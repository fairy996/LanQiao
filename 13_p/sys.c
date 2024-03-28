#include "sys.h"

void Init_Hc138(unsigned char channel)
{
    switch (channel)
    {
    case 4:
        P2 = (P2 & 0x1F) | 0x80;
        break;
    case 5:
        P2 = (P2 & 0x1F) | 0xA0;
        break;
    case 6:
        P2 = (P2 & 0x1F) | 0xC0;
        break;
    case 7:
        P2 = (P2 & 0x1F) | 0xE0;
        break;
    }
    P2 = P2 & 0x1F;
}

void Init_Sys(void)
{
    P0 = 0xFF;
    Init_Hc138(4);
    P0 = 0x00;
    Init_Hc138(5);
}

void Delay_us(unsigned int us) //@12.000MHz
{
    while (us--)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void Delay_ms(unsigned int ms) //@12.000MHz
{
    while (ms--)
    {
        unsigned char data i, j;

        i = 12;
        j = 169;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}
