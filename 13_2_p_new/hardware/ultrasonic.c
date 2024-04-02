#include "ultrasonic.h"

sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;

void Send_Pluse()
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
    uint time = 0;
    TMOD &= 0x0f;  // 配置定时器1计时模式
    TH1 = TL1 = 0; // 复位计数值 等待超声波信号发出
    Send_Pluse();
    TR1 = 1;
    while (RX == 1 && TF1 == 0)
        ;
    TR1 = 0;
    if (TF0 == 0)
    {
        time = (TH1 << 8) | TL1;
        return (time * 0.017);
    }
    else
    {
        TF1 = 0;
        return 999;
    }
}