#include "ultrasonic.h"

sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;


void Send_Pulses()
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        TX = 1;
        Delay12us();
        TX = 0;
        Delay12us();
    }
}

unsigned int measure_distance()
{
    uint time;
    TH0 = TL0 = 0;
    Send_Pulses();
    TR0 = 1;
    while (RX == 1 && TF0 == 0);
    TR0 = 0;
    if(TF0 == 0)
    {
        time = (TH0 << 8) | TL0;
        return (time * 0.017);
    }
    else
    {
        TF0 = 0;
        return 999;
    }
}