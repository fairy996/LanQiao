#include "sys.h"

void Delay12us(void) //@12.000MHz
{
    unsigned char data i;

    _nop_();
    _nop_();
    i = 33;
    while (--i)
        ;
}

void InitHC138(unsigned char channel)
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

void Init_Sys()
{
    P0 = 0xFF;
    InitHC138(4);
    P0 = 0x00;
    InitHC138(5);
}