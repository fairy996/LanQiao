#include "sys.h"

void InitHC138(unsigned char channel)
{
    switch(channel)
    {
        case 4:P2 = (P2 & 0x1f) | 0x80;break;
        case 5:P2 = (P2 & 0x1f) | 0xa0;break;
        case 6:P2 = (P2 & 0x1f) | 0xc0;break;
        case 7:P2 = (P2 & 0x1f) | 0xe0;break;
    }
    P2 = P2 & 0x1f;
}

void Delayus(unsigned int us)	//@11.0592MHz
{
    while (us--)
    {
        /* code */
        _nop_();
	    _nop_();
	    _nop_();
    }
}

void Delayms(unsigned int ms)	//@11.0592MHz
{
    while(ms--)
    {
        unsigned char data i, j;

        _nop_();
        _nop_();
        _nop_();
        i = 11;
        j = 190;
        do
        {
            while (--j);
        } while (--i);
    }
}

void Init_Sys()
{
	P0 = 0xFF;
    InitHC138(4);
    P0 = 0x00;
    InitHC138(5);
}
