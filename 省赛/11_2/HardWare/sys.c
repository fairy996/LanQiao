#include "sys.h"

void Init_HC138(unsigned char channel)
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
    Init_HC138(4);
    P0 = 0x00;
    Init_HC138(5);
}