#include "sys.h"

//**********************变量定义******************************//
// 按键
unsigned int Key_Slow_Down;
unsigned char Key_Val;
unsigned char Key_Up;
unsigned char Key_Down;
unsigned char Key_Old;
// 数码管
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Position;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char LED[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// 测量数据
unsigned char temp_para = 23;
unsigned int temp;
unsigned char Time_Now[3] = {24, 30, 30};
// 时间数据
unsigned char Time[3] = {07, 59, 30};
unsigned int time_5s;
unsigned char time_100ms;

// 标志位
unsigned char state = 0; // 界面标志 0为温度 1为时间 2为参数
bit mode = 0;  // 控制模式 0为温度 1为时间
bit L3_Light_Flag = 0;
bit Time_View_Flag = 0; // 0表示显示时和分,1表示显示分和秒
bit Int_Time_Flag = 0;  // 表示当前为整点
bit Trigger_Flag = 0;   // 0表示此时继电器未达到触发状态，1表示继电器吸合
void Clear_NiXie(void)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        NiXie_Value[i] = 16;
        NiXie_Point[i] = 0;
    }
}

void key_pro()
{
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Scan_Matrix();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;

    switch (Key_Down)
    {
    case 12:
        Clear_NiXie();
        if (state == 0)
            state = 1;
        else if (state == 1)
            state = 2;
        else if (state == 2)
            state = 0;
        break;
    case 13:
        Clear_NiXie();
        mode = ~mode;
        break;
    case 16:
        Clear_NiXie();
        if (state == 2)
        {
            temp_para += 1;
            if(temp_para > 99)
            {
                temp_para = 99;
            }
        }
        break;
    case 17:
        Clear_NiXie();
        if (state == 2)
        {
            temp_para -= 1;
            if (temp_para < 10)
            {
                temp_para = 10;
            }
        }
        else if (state == 1)
        {
            Time_View_Flag = 1;
        }
        
        break;
    }
    switch(Key_Up)
    {
        case 17:
            Time_View_Flag = 0;
            break;
    }
}

void NiXie_pro()
{
    if (NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;

    temp = Read_Temperature();
    DS1302_Time_Read(Time_Now);
    
    NiXie_Value[0] = 17;
    NiXie_Value[1] = state + 1;
    if (state == 0)
    {

        NiXie_Value[5] = temp / 100;
        NiXie_Value[6] = temp % 100 / 10;
        NiXie_Value[7] = temp % 10;
        NiXie_Point[6] = 1;
    }
    else if (state == 1)
    {
        if (Time_View_Flag)
        {
            NiXie_Value[3] = Time_Now[1] / 10;
            NiXie_Value[4] = Time_Now[1] % 10;
            NiXie_Value[5] = 18;
            NiXie_Value[6] = Time_Now[2] / 10;
            NiXie_Value[7] = Time_Now[2] % 10;
        }
        else
        {
            NiXie_Value[3] = Time_Now[0] / 10;
            NiXie_Value[4] = Time_Now[0] % 10;
            NiXie_Value[5] = 18;
            NiXie_Value[6] = Time_Now[1] / 10;
            NiXie_Value[7] = Time_Now[1] % 10;
        }
    }
    else if (state == 2)
    {
        NiXie_Value[6] = temp_para / 10;
        NiXie_Value[7] = temp_para % 10;
    }
}

void Led_pro()
{
    if (Time_Now[1] == 0 && Time_Now[2] == 0 && Int_Time_Flag == 0)
        Int_Time_Flag = 1;
    if(mode == 0)
    {
        Trigger_Flag = (temp > temp_para * 10);
    }
    else
    {
        Trigger_Flag = Int_Time_Flag;
    }
    LED[0] = Int_Time_Flag;
    LED[1] = (mode == 0);
    LED[2] = (Trigger_Flag) ? L3_Light_Flag : 0;
}

void Timer0_Isr(void) interrupt 1
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_Slow_Down == 50)
        NiXie_Slow_Down = 0;
    if (++NiXie_Position == 8)
        NiXie_Position = 0;
    if (++time_100ms == 100)
    {
        time_100ms = 0;
        L3_Light_Flag = ~L3_Light_Flag;
    }
    if(++time_5s == 5000)
    {
        time_5s = 0;
        Int_Time_Flag = 0;
    }
    NiXie_Display(NiXie_Position, NiXie_Value[NiXie_Position], NiXie_Point[NiXie_Position]);
    LED_Display(NiXie_Position, LED[NiXie_Position]);
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
    EA = 1;       // 使能总中断
}

void main()
{
    temp = Read_Temperature();
    while (temp == 850)
        temp = Read_Temperature();
    Init_Sys();
    Timer0_Init();
    DS1302_Time_Write(Time);
    while (1)
    {
        key_pro();
        Led_pro();
        NiXie_pro();
    }
}
