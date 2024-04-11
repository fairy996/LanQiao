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

void Ut_Wave_Init() // 超声波初始化函数 产生8个40Mhz的方波信号
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        TX = 1;
        Delay12us();
        TX = 0;
        Delay12us();
    }
}

unsigned char Measure_Distance() // 超声波距离读取函数
{
    unsigned int time; // 时间储存变量
    TMOD &= 0x0f;      // 配置定时器1计时模式
    TMOD |= 0x50;
    TH1 = TL1 = 0;     // 复位计数值 等待超声波信号发出
    Ut_Wave_Init();    // 发送超声波信号
    TR1 = 1;           // 开始计时
    while ((RX == 1) && (TF1 == 0));         // 等待接受返回信号或者定时器溢出
    TR1 = 0;      // 停止计时
    if (TF1 == 0) // 定时器没有溢出
    {
        time = TH1 << 8 | TL1; // 读取当前时间
        return (time * speed / 2000000) + Calibration;
    }
    else
    {
        TF1 = 0; // 清除溢出标志位
        return (65536 * speed / 2000000) + Calibration;
    }
}