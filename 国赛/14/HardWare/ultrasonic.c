#include "ultrasonic.h"
sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;
void Delay12us(void) //@12.000MHz
{
    unsigned char data i;

    _nop_();
    _nop_();
    i = 33;
    while (--i)
        ;
}

void Send_Pluse()
{
    unsigned char i;
    for(i = 0;i<8;i++)
    {
        TX = 1;
        Delay12us();
        TX = 0;
        Delay12us();
    }
}

unsigned int Measure_Distance()
{
    //unsigned int temp;
    TH1 = TL1 = 0;
    Send_Pluse();
    TR1 = 1;
    while((TF1 == 0) && (RX == 1))
        ;
    TR1 = 0;
    if(TF1 == 1)
    {
        TF1 = 0;
        return 65535;
    }
    else
    {
        return (TH1 << 8) | TL1;
    }
}