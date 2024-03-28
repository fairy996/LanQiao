#include "ultr.h"

sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;

void Delay13us(void) //@12.000MHz
{
    unsigned char data i;

    _nop_();
    _nop_();
    i = 36;
    while (--i)
    {
        nixie_pro();
        // NiXie_Control(nixie_data, nixie_pos);
    }
}

void SendPluse()
{
    unsigned int i;
    for (i = 0; i < 8; i++)
    {
        TX = 1;
        Delay13us(13);
        TX = 0;
        Delay13us(13);
    }
}

unsigned int ultr_read()
{
    unsigned int num = 0;
    unsigned int distance = 0;
    unsigned int time = 0;
    TH0 = TL0 = 0x00;
    send_Pluse();
    TR0 = 1;
    // RX == 0---->超声波返回，TF1 = 1------>秒表溢出
    while (RX == 1 && TF0 == 0)
        ;
    TR0 = 0;
    if (TF0 == 0)
    {
        num = TH0 << 8 | TL0;
        // time = num * 1/12/2;//us
        distance = num * 0.017; // 距离以厘米为单位
    }
    else if (TF0 == 1)
    {
        distance = 999;
        TF0 = 0;
    }
    return distance;
}