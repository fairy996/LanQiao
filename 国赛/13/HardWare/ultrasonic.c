#include "ultrasonic.h"

sbit Tx = P1 ^ 0;
sbit Rx = P1 ^ 1;

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
    for (i = 0; i < 8; i++)
    {
        Tx = 1;
        Delay12us();
        Tx = 0;
        Delay12us();
    }
}

unsigned int Measure_Distance()
{
    unsigned int temp;
    //AUXR &= 0x7F; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式
    TH0 = TL0 = 0x00;
    Send_Pluse();
    TR0 = 1;
    while((TF0 == 0)&&(Rx == 1))
        ;
    TR0 = 0;
    if(TF0 == 0)
    {
        temp = (TH0 << 8) | TL0;
        return temp * 0.017;
    }
    else
    {
        TF0 = 0;
        return 999;
    }
}