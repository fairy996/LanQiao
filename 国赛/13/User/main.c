#include "sys.h"
//****************参数*********************
// 按键
unsigned char Key_Slow_Down;
// 数码管
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Pos;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Dot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char LED_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// 时间变量
unsigned char time_100ms;
//unsigned int time_500ms;
unsigned int time_1s;
unsigned int time_800us;
// 数据
unsigned char Fre_Para = 90;
unsigned char Hum_Para = 40;
unsigned char Dis_Para = 6;
unsigned int Fre;
unsigned char Hum;
unsigned int Dis;
unsigned char Dis_Count;
// 标志位
unsigned char state = 0;      // 0 频率 1湿度 2 测距 3 参数
unsigned char para_state = 0; // 0 频率 1湿度 2 距离
bit Led_Light;
bit Dis_Unit = 0; // 0 cm 1 m
bit Fre_Unit = 0; // 0 Hz 1 kHz
bit Relay_Flag = 0;
bit PWM_Flag = 0;//频率小于频率参数
//********************************************************************
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
    unsigned char Key_Val,Key_Down,Key_Up,Key_Old;
    if (Key_Slow_Down)
        return;
    Key_Slow_Down = 1;
    
    
    Key_Val = Key_Scan();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;

    switch (Key_Down)
    {
    case 4:
        Clear_NiXie();
        state += 1;
        state %= 4;
        if(state == 3)
            para_state = 0;
        break;
    case 5:
        Clear_NiXie();
        if (state == 3)
        {
            para_state += 1;
            para_state %= 3;
        }
        break;
    case 6:
        Clear_NiXie();
        if (state == 3)
        {
            if (para_state == 0)
            {
                Fre_Para += 5;
                if (Fre_Para > 120)
                    Fre_Para = 10;
            }
            else if (para_state == 1)
            {
                Hum_Para += 10;
                if (Hum_Para > 60)
                    Hum_Para = 10;
            }
            else if (para_state == 2)
            {
                Dis_Para += 1;
                if (Dis_Para > 12)
                    Dis_Para = 1;
            }
        }
        else if (state == 2)
            Dis_Unit = ~Dis_Unit;
        break;
    case 7:
        Clear_NiXie();
        if (state == 3)
        {
            if (para_state == 0)
            {
                Fre_Para -= 5;
                if (Fre_Para < 10)
                    Fre_Para = 120;
            }
            else if (para_state == 1)
            {
                Hum_Para -= 10;
                if (Hum_Para < 10)
                    Hum_Para = 60;
            }
            else if (para_state == 2)
            {
                Dis_Para -= 1;
                if (Dis_Para < 1)
                    Dis_Para = 12;
            }
        }
        else if (state == 0)
            Fre_Unit = ~Fre_Unit;
        break;
    }
}

void NiXie_pro()
{
    unsigned char dac_out;
    if (NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;
    if(state == 0)
    {
        TR0 = 1;
        TMOD &= 0xF0; // 设置定时器模式
        TMOD |= 0x05; // 设置定时器模式
    }
    else if (state == 1)
        Hum = adc_read(0x03) * 100 / 255 ;
    else if(state == 2)
    {
        TF1 = 0;
        TR0 = 0;
        Dis = Measure_Distance();
    }
    if(Hum < Hum_Para)
        dac_out = 51;
    else if(Hum > 80)
        dac_out = 255;
    else
        dac_out = (Hum - Hum_Para) * (4 / (80 - Hum_Para)) + 51;
    dac_output(dac_out);
    Dis_Count = (Dis > Dis_Para * 10) ? Dis_Count + 1 : Dis_Count;
    Write_AT24C02(0x00, Dis_Count);
    PWM_Flag = (Fre > Fre_Para * 1000) ? 1 : 0;
    switch (state)
    {
    case 0:
        NiXie_Value[0] = 15;
        if (Fre_Unit == 0)
        {
            NiXie_Value[2] = (Fre / 100000) ? (Fre / 100000) : 16;
            if (NiXie_Value[2] != 16)
            {
                NiXie_Value[3] = (Fre % 100000) / 10000;
                NiXie_Value[4] = (Fre % 10000) / 1000;
                NiXie_Value[5] = (Fre % 1000) / 100;
                NiXie_Value[6] = (Fre % 100) / 10;
                NiXie_Value[7] = (Fre % 10);
            }
            else
            {
                NiXie_Value[3] = ((Fre % 100000) / 10000) ? ((Fre % 100000) / 10000) : 16;
                if (NiXie_Value[3] != 16)
                {
                    NiXie_Value[4] = (Fre % 10000) / 1000;
                    NiXie_Value[5] = (Fre % 1000) / 100;
                    NiXie_Value[6] = (Fre % 100) / 10;
                    NiXie_Value[7] = (Fre % 10);
                }
                else
                {
                    NiXie_Value[4] = ((Fre % 10000) / 1000) ? ((Fre % 10000) / 1000) : 16;
                    if (NiXie_Value[4] != 16)
                    {
                        NiXie_Value[5] = (Fre % 1000) / 100;
                        NiXie_Value[6] = (Fre % 100) / 10;
                        NiXie_Value[7] = (Fre % 10);
                    }
                    else
                    {
                        NiXie_Value[5] = ((Fre % 1000) / 1000) ? ((Fre % 1000) / 1000) : 16;
                        if (NiXie_Value[5] != 16)
                        {
                            NiXie_Value[6] = (Fre % 100) / 10;
                            NiXie_Value[7] = (Fre % 10);
                        }
                        else
                        {
                            NiXie_Value[6] = ((Fre % 1000) / 1000) ? ((Fre % 1000) / 1000) : 16;
                            NiXie_Value[7] = (Fre % 10);
                        }
                    }
                }
            }
        }
        else
        {
            if (Fre >= 1000)
            {
                NiXie_Value[5] = (Fre / 100000) ? (Fre / 100000) : 16;
                NiXie_Value[6] = (Fre % 10000) / 1000;
            }
            else
            {
                NiXie_Value[6] = 0;
            }
            if ((Fre % 100) > 50)
                NiXie_Value[7] = (Fre % 1000) / 100 + 1;
            else
                NiXie_Value[7] = (Fre % 1000) / 100;
            NiXie_Dot[6] = 1;
        }
        break;
    case 1:
        NiXie_Value[0] = 17;
        NiXie_Value[6] = (Hum / 10) ? (Hum / 10) : 16;
        NiXie_Value[7] = Hum % 10;
        break;
    case 2:
        NiXie_Value[0] = 10;
        if (Dis_Unit)
        {
            NiXie_Value[5] = (Dis / 100);
            NiXie_Value[6] = (Dis % 100) / 10;
            NiXie_Value[7] = (Dis % 10);
            NiXie_Dot[5] = 1;
        }
        else
        {
            NiXie_Value[5] = (Dis / 100) ? (Dis / 100) : 16;
            if (NiXie_Value[5] != 16)
            {
                NiXie_Value[6] = (Dis % 100) / 10;
                NiXie_Value[7] = (Dis % 10);
            }
            else
            {
                NiXie_Value[6] = ((Dis % 100) / 10) ? ((Dis % 100) / 10) : 16;
                NiXie_Value[7] = (Dis % 10);
            }
        }
        break;

    case 3:
        NiXie_Value[0] = 18;
        NiXie_Value[1] = para_state + 1;
        if (para_state == 0)
        {
            NiXie_Value[5] = (Fre_Para / 100) ? (Fre_Para / 100) : 16;
            NiXie_Value[6] = (Fre_Para % 100) / 10;
            NiXie_Value[7] = (Fre_Para % 10);
            NiXie_Dot[6] = 1;
        }
        else if (para_state == 1)
        {
            NiXie_Value[6] = Hum_Para / 10;
            NiXie_Value[7] = Hum_Para % 10;
        }
        else if (para_state == 2)
        {
            NiXie_Value[6] = Dis_Para / 10;
            NiXie_Value[7] = Dis_Para % 10;
            NiXie_Dot[6] = 1;
        }
        break;
    }
}

void LED_pro()
{
    Relay_Flag = (Dis > Dis_Para*10) ? 1 : 0;
    if (state == 3)
    {
        LED_Value[0] = (para_state == 0) ? Led_Light : 0;
        LED_Value[1] = (para_state == 1) ? Led_Light : 0;
        LED_Value[2] = (para_state == 2) ? Led_Light : 0;
    }
    else
    {
        LED_Value[0] = (state == 0);
        LED_Value[1] = (state == 1);
        LED_Value[2] = (state == 2);
    }
    LED_Value[3] = (Fre > Fre_Para * 1000) ? 1 : 0;
    LED_Value[4] = (Hum > Hum_Para) ? 1 : 0;
    LED_Value[5] = (Dis > Dis_Para * 10) ? 1 : 0;
}

void Timer1_Isr(void) interrupt 3
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_Slow_Down == 200)
        NiXie_Slow_Down = 0;
    if (++NiXie_Pos == 8)
        NiXie_Pos = 0;
    if (++time_100ms == 100)
    {
        time_100ms = 0;
        Led_Light = ~Led_Light;
    }
    if(++time_1s == 1000)
    {
        time_1s = 0;
        Fre = (TH0 << 8) | TL0;
        TH0 = 0;
        TL0 = 0;
    }
    NiXie_Display(NiXie_Pos, NiXie_Value[NiXie_Pos], NiXie_Dot[NiXie_Pos]);
    LED_Display(NiXie_Pos, LED_Value[NiXie_Pos]);
    Relay(Relay_Flag);
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

   // TMOD |= 0x05; //设置为计数器
    TL0 = 0x00;   // 设置定时初始值
    TH0 = 0x00;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 0;      // 定时器0开始计时
}
void Timer2_Isr(void) interrupt 12
{
    if(++time_800us == 8)
    {
        time_800us = 0;
        if(PWM_Flag)
        {
            P0 = 0x20;
        } 
        else
        {
            P0 = 0x00;
        }
    }
}

void Timer2_Init(void) // 100微秒@12.000MHz
{
    AUXR &= 0xFB; // 定时器时钟12T模式
    T2L = 0x9C;   // 设置定时初始值
    T2H = 0xFF;   // 设置定时初始值
    AUXR |= 0x10; // 定时器2开始计时
    IE2 |= 0x04;  // 使能定时器2中断
}

void main()
{
    Init_Sys();
    Timer1_Init();
    Timer2_Init();
    // Timer0_Init();
    while (1)
    {
        Key_pro();
        NiXie_pro();
        LED_pro();
    }
}