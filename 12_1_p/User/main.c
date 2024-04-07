#include "sys.h"

//**********参数设置******************
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
unsigned char NiXie_Dot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char LED_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// 时间

// 测量数据
unsigned char temp_para = 25;
unsigned int temp_val;
unsigned char Digital_DAC;
unsigned int Analog_DAC;
// 标志位
unsigned char state = 0; // 0 温度显示界面 1 参数设置界面 2 DAC输出界面
bit mode = 0;            // 0 模式1 1 模式2
void Clear_NiXie()
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        NiXie_Value[i] = 16;
        NiXie_Dot[i] = 0;
    }
}

void Key_pro()
{
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Scan();
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Down = ~Key_Val & (Key_Val ^ Key_Old);
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
    case 8:
        Clear_NiXie();
        if (state == 1)
            temp_para -= 1;
        break;
    case 9:
        Clear_NiXie();
        if (state == 1)
            temp_para += 1;
        break;
    case 5:
        Clear_NiXie();
        mode = ~mode;
        break;
    }
}

void NiXie_pro()
{
    if (NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;

    temp_val = Read_temperature();
    if((mode == 0) && (state == 1))
    {
        if(temp_val < temp_para * 100)
            Digital_DAC = 0;
        else if(temp_val > temp_para * 100)
            Digital_DAC = 255;
    }
    else
    {
        Digital_DAC = 51 + (153 / 20) * ((temp_val / 100) - 20);
    }
    Analog_DAC = (Digital_DAC * 500) / 255;
    if (state == 0)
    {
        NiXie_Value[0] = 12;
        NiXie_Value[4] = (temp_val / 1000) ? (temp_val / 1000) : 16;
        NiXie_Value[5] = ((temp_val % 1000) / 100);
        NiXie_Value[6] = ((temp_val % 100) / 10);
        NiXie_Value[7] = temp_val % 10;
        NiXie_Dot[5] = 1;
    }
    else if(state == 1)
    {
        NiXie_Value[0] = 17;
        NiXie_Value[6] = temp_para / 10;
        NiXie_Value[7] = temp_para % 10;
    }
    else if(state == 2)
    {
        NiXie_Value[0] = 10;
        NiXie_Value[5] = Analog_DAC / 100;
        NiXie_Value[6] = (Analog_DAC % 100) / 10;
        NiXie_Value[7] = Analog_DAC % 10;
        NiXie_Dot[5] = 1;
    }
}

void Led_pro()
{
    LED_Value[0] = ~mode;
    LED_Value[1] = (state == 0);
    LED_Value[2] = (state == 1);
    LED_Value[3] = (state == 3);
}

void Timer0_Isr(void) interrupt 1
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_Slow_Down == 10)
        NiXie_Slow_Down = 0;
    if (++NiXie_Location == 8)
        NiXie_Location = 0;

    NiXie_Display(NiXie_Location, NiXie_Value[NiXie_Location], NiXie_Dot[NiXie_Location]);
    LED_Display(NiXie_Location, LED_Value[NiXie_Location]);
}

void Timer0_Init(void) // 1毫秒@12.000MHz
{
    AUXR &= 0x7F; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0x18;   // 设置定时初始值
    TH0 = 0xFC;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
    ET0 = 1;      // 使能定时器0中断
    EA = 1;
}

void main()
{
    while(Read_temperature() >= 8500)
        ;
    Init_Sys();
    Timer0_Init();

    while (1)
    {
        Key_pro();
        NiXie_pro();
        Led_pro();
    }
}