#include "sys.h"

//**********************参数设置************************
// 按键
unsigned int Key_Slow_Down;
unsigned char Key_Val;
unsigned char Key_Up;
unsigned char Key_Down;
unsigned char Key_Old;
// 数码管
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Location;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char LED_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// 数据
unsigned int V_Val_A;          // 实时采集的电压模拟量
unsigned char V_Val_D;         // 实时采集的电压数字量
unsigned char V_Channel3_Val;  // 通道3当前电压
unsigned char V_Channel3_Save; // 通道3缓存电压
unsigned int Frequence_Val;    // 当前采集的频率
unsigned int Frequence_Save;   // 缓存的频率
unsigned int period;
// 时间变量
unsigned int time_1s;
unsigned int time_led;
// 标志位
unsigned char state = 0;           // 0 频率 1 周期 2 电压
unsigned char Channel = 0;                   // 0 光敏 1 电位器
unsigned char LED_Flag = 1;                  // 1 LED显示  0 LED关闭
unsigned char long_press_flag = 0; // 长按开始计数标志
void Clear_NiXie()
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        NiXie_Value[i] = 16;
        NiXie_Point[i] = 0;
    }
}

void Key_pro()
{
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Scan();
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
    Key_Old = Key_Val;

    switch (Key_Down)
    {
    case 4:
        Clear_NiXie();
        if (state == 0)
            state = 1;
        else if (state == 1)
            state = 2;
        else if (state == 2)
            state = 0;
        break;
    case 5:
        Clear_NiXie();
        if (state == 2)
            Channel = ~Channel;
        break;
    case 6:
        Clear_NiXie();
        V_Channel3_Save = V_Channel3_Val;
        break;
    case 7:
        Clear_NiXie();
        Frequence_Save = Frequence_Val;
        long_press_flag = 1;
        break;
    }
    switch (Key_Up)
    {
    case 7:
        Clear_NiXie();
        if (time_led >= 1000)
            LED_Flag = ~LED_Flag;
        long_press_flag = 0;
        break;
    }
}

void NiXie_pro()
{
    if (NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;

    if (state == 0)
    {
        NiXie_Value[0] = 15;
        NiXie_Value[1] = (Frequence_Val / 1000000) ? (Frequence_Val / 1000000) : 16;
        NiXie_Value[2] = ((Frequence_Val % 1000000) / 100000) ? ((Frequence_Val % 1000000) / 100000) : 16;
        NiXie_Value[3] = ((Frequence_Val % 100000) / 10000) ? ((Frequence_Val % 100000) / 10000) : 16;
        NiXie_Value[4] = ((Frequence_Val % 10000) / 1000) ? ((Frequence_Val % 10000) / 1000) : 16;
        NiXie_Value[5] = ((Frequence_Val % 1000) / 100) ? ((Frequence_Val % 1000) / 100) : 16;
        NiXie_Value[6] = ((Frequence_Val % 100) / 10) ? ((Frequence_Val % 100) / 10) : 16;
        NiXie_Value[7] = (Frequence_Val % 10) ? (Frequence_Val % 10) : 16;
    }
    else if (state == 1)
    {
        NiXie_Value[0] = 17;
        NiXie_Value[1] = (period / 1000000) ? (period / 1000000) : 16;
        NiXie_Value[2] = ((period % 1000000) / 100000) ? ((period % 1000000) / 100000) : 16;
        NiXie_Value[3] = ((period % 100000) / 10000) ? ((period % 100000) / 10000) : 16;
        NiXie_Value[4] = ((period % 10000) / 1000) ? ((period % 10000) / 1000) : 16;
        NiXie_Value[5] = ((period % 1000) / 100) ? ((period % 1000) / 100) : 16;
        NiXie_Value[6] = ((period % 100) / 10) ? ((period % 100) / 10) : 16;
        NiXie_Value[7] = (period % 10) ? (period % 10) : 16;
    }
    else if (state == 2)
    {
        NiXie_Value[0] = 18;
        NiXie_Value[1] = 19;
        NiXie_Value[2] = (Channel) ? 3 : 1;
        NiXie_Value[5] = V_Val_A / 100;
        NiXie_Value[6] = (V_Val_A % 100) / 10;
        NiXie_Value[7] = V_Val_A % 10;
        NiXie_Point[5] = 1;
    }
}

void Led_pro()
{
    if (~Channel)
        V_Val_D = adc_read(0x01);
    else
        V_Val_D = adc_read(0x03);
    V_Val_A = (V_Val_D * 100) / 255 * 5;
    LED_Value[0] = ((V_Channel3_Val > V_Channel3_Save) && (LED_Flag));
    LED_Value[1] = ((Frequence_Val > Frequence_Save) && (LED_Flag));
    LED_Value[2] = ((state == 0) && (LED_Flag));
    LED_Value[3] = ((state == 1) && (LED_Flag));
    LED_Value[4] = ((state == 2) && (LED_Flag));
}

void Timer1_Isr(void) interrupt 3
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_Slow_Down == 200)
        NiXie_Slow_Down = 0;
    if (++NiXie_Location == 8)
        NiXie_Location = 0;

    if (++time_1s == 1000)
    {
        time_1s = 0;
        Frequence_Val = (TH0 << 8) | TL0;
        period = 1000000 / Frequence_Val;
        TH0 = 0;
        TL0 = 0;
    }
    if (long_press_flag)
    {
        if (++time_led >= 1000)
        {
            time_led = 1001;
        }
    }
    NiXie_Display(NiXie_Location, NiXie_Value[NiXie_Location], NiXie_Point[NiXie_Location]);
    LED_Display(NiXie_Location, LED_Value[NiXie_Location]);
}

void Timer1_Init(void) // 1毫秒@12.000MHz
{
    AUXR &= 0xBF; // 定时器时钟12T模式
    TMOD &= 0x0F; // 设置定时器模式
    TL1 = 0x18;   // 设置定时初始值
    TH1 = 0xFC;   // 设置定时初始值
    TF1 = 0;      // 清除TF1标志
    TR1 = 1;      // 定时器1开始计时
    ET1 = 1;      // 使能定时器1中断
    EA = 1;
}

void Timer0_Init(void) // 1毫秒@12.000MHz
{
    AUXR &= 0x7F; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式
    TMOD |= 0x05; // 设置定时器模式
    TL0 = 0;      // 设置定时初始值
    TH0 = 0;      // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
}

void main()
{
    Init_Sys();
    Timer0_Init();
    Timer1_Init();

    while (1)
    {
        Led_pro();
        NiXie_pro();
        Key_pro();
    }
}