#include "sys.h"

//************参数**************
// 按键
unsigned int Key_Slow_Down;
unsigned char Key_Val;
unsigned char Key_Down;
unsigned char Key_Up;
unsigned char Key_Old;
// 数码管
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Location;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Dot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char Led_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// 时间变量
unsigned char time_100ms;
unsigned char time_500ms;
// 数据
unsigned int temp_val;
unsigned int distance_val;
unsigned char temp_para;
unsigned char distance_para;
char Calibration;
unsigned int speed;
unsigned char dac_limit;
unsigned char dac_out;
// 标志位
bit distance_unit = 0;           // 0 cm 1 m
bit para_flag = 0;     // 0 距离参数 1 温度参数
unsigned char factory_state = 0; // 0 校准值 1 介质设置 2 DAC输出设置
unsigned char state = 340;       // 0 测距 1 参数 2 工厂
unsigned char Led_light;
unsigned char Relay_flag = 0; // 0断开 1吸合
void Init_State()
{
    state = 0;
    distance_unit = 0;
    temp_para = 30;
    distance_para = 40;
    Calibration = 0;
    speed = 340;
    dac_limit = 10;
}
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
    Key_Down = Key_Val & (Key_Old ^ Key_Val); // 捕捉按键下降沿
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);  // 捕捉按键上降沿
    Key_Old = Key_Val;

    switch (Key_Down)
    {
    case 4:
        Clear_NiXie();
        if (state == 0)
        {
            state = 1;
        }
        else if (state == 1)
        {
            state = 2;
        }

        else if (state == 2)
        {
            state = 0;
        }
        break;
    case 5:
        Clear_NiXie();
        if (state == 0)
        {
            distance_unit = ~distance_unit;
        }
        else if (state == 1)
        {
            para_flag ^= 1;
        }

        else if (state == 2)
        {
            if (factory_state == 0)
                factory_state = 1;
            else if (factory_state == 1)
                factory_state = 2;
            else if (factory_state == 2)
                factory_state = 0;
        }
        break;
    case 8:
        Clear_NiXie();
        if (state == 1)
        {
            if (para_flag)
            {
                if (++temp_para == 80)
                {
                    temp_para = 80;
                }
            }
            else
            {
                distance_para += 10;
                if (distance_para >= 99)
                {
                    distance_para = 99;
                }
            }
        }
        else if (state == 2)
        {
            if (factory_state == 0)
            {
                Calibration += 5;
                if (Calibration >= 90)
                {
                    Calibration = 90;
                }
            }
            else if (factory_state == 1)
            {
                speed += 10;
                if (speed >= 9990)
                {
                    speed = 9990;
                }
            }
            else if (factory_state == 2)
            {
                if (++dac_limit == 20)
                {
                    dac_limit = 20;
                }
            }
        }
        break;
    case 9:
        Clear_NiXie();
        if (state == 1)
        {
            if (para_flag)
            {
                if (--temp_para == 0)
                {
                    temp_para = 0;
                }
            }
            else
            {
                distance_para -= 10;
                if (distance_para <= 10)
                {
                    distance_para = 10;
                }
            }
        }
        else if (state == 2)
        {
            if (factory_state == 0)
            {
                Calibration -= 5;
                if (Calibration <= -90)
                {
                    Calibration = -90;
                }
            }
            else if (factory_state == 1)
            {
                speed -= 10;
                if (speed <= 10)
                {
                    speed = 10;
                }
            }
            else if (factory_state == 2)
            {
                if (--dac_limit == 1)
                {
                    dac_limit = 1;
                }
            }
        }
        break;
    }
}

void NiXie_pro()
{
    if (NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;

    // temp_val = Read_Temperature();

    if ((distance_val >= distance_para + 5) && (distance_val <= distance_para + 5) && (temp_val <= temp_para * 10))
    {
        Relay_flag = 1;
    }
    else
    {
        Relay_flag = 0;
    }
    if (state == 0)
    {
        NiXie_Value[0] = temp_val / 100;
        NiXie_Value[1] = (temp_val % 100) / 10;
        NiXie_Value[2] = temp_val % 10;
        NiXie_Dot[1] = 1;
        NiXie_Value[3] = 18;
        NiXie_Value[4] = (distance_val / 1000) ? (distance_val / 1000) : 16;
        if (distance_val / 1000)
        {
            NiXie_Value[5] = ((distance_val % 1000) / 100) ? ((distance_val % 1000) / 100) : 0;
            NiXie_Value[6] = ((distance_val % 100) / 10) ? ((distance_val % 100) / 10) : 0;
            NiXie_Value[7] = (distance_val % 10) ? (distance_val % 10) : 0;
        }
        else
        {
            if (distance_unit)
            {
                NiXie_Value[5] = ((distance_val % 1000) / 100) ? ((distance_val % 1000) / 100) : 0;
                NiXie_Value[6] = ((distance_val % 100) / 10) ? ((distance_val % 100) / 10) : 0;
                NiXie_Value[7] = (distance_val % 10) ? (distance_val % 10) : 0;
            }
            else
            {
                NiXie_Value[5] = ((distance_val % 1000) / 100) ? ((distance_val % 1000) / 100) : 16;
                if ((distance_val % 1000) / 100)
                {
                    NiXie_Value[6] = ((distance_val % 100) / 10) ? ((distance_val % 100) / 10) : 0;
                    NiXie_Value[7] = (distance_val % 10) ? (distance_val % 10) : 0;
                }
                else
                {
                    NiXie_Value[6] = ((distance_val % 100) / 10) ? ((distance_val % 100) / 10) : 16;
                    NiXie_Value[7] = (distance_val % 10) ? (distance_val % 10) : 0;
                }
            }
        }
        NiXie_Dot[5] = (distance_unit);
    }
    else if (state == 1)
    {
        NiXie_Value[0] = 17;
        if (para_flag == 1)
        {
            NiXie_Value[1] = 2;
            NiXie_Value[6] = temp_para / 10;
            NiXie_Value[7] = temp_para % 10;
        }
        else
        {
            NiXie_Value[1] = 1;
            NiXie_Value[6] = distance_para / 10;
            NiXie_Value[7] = distance_para % 10;
        }
    }
    else if (state == 2)
    {
        NiXie_Value[0] = 15;
        NiXie_Value[1] = factory_state + 1;
        if (factory_state == 0)
        {
            if (Calibration < -10)
            {
                NiXie_Value[5] = 18;
                NiXie_Value[6] = (-Calibration / 10);
                NiXie_Value[7] = (-Calibration % 10);
            }
            else if (Calibration < 0)
            {
                NiXie_Value[5] = 16;
                NiXie_Value[6] = 18;
                NiXie_Value[7] = (-Calibration);
            }
            else
            {
                NiXie_Value[5] = (Calibration / 100) ? (Calibration / 100) : 16;
                if (Calibration / 100)
                {
                    NiXie_Value[6] = ((Calibration % 100) / 10) ? ((Calibration % 100) / 10) : 0;
                    NiXie_Value[7] = Calibration % 10;
                }
                else
                {
                    NiXie_Value[6] = ((Calibration % 100) / 10) ? ((Calibration % 100) / 10) : 16;
                    NiXie_Value[7] = Calibration % 10;
                }
            }
        }
        else if (factory_state == 1)
        {
            NiXie_Value[4] = (speed / 1000) ? (speed / 1000) : 16;
            if (speed / 1000)
            {
                NiXie_Value[5] = ((speed % 1000) / 100) ? ((speed % 1000) / 100) : 0;
                NiXie_Value[6] = ((speed % 100) / 10) ? ((speed % 100) / 10) : 0;
                NiXie_Value[6] = speed % 10;
            }
            else
            {
                NiXie_Value[5] = ((speed % 1000) / 100) ? ((speed % 1000) / 100) : 16;
                if ((speed % 1000) / 100)
                {
                    NiXie_Value[6] = ((speed % 100) / 10) ? ((speed % 100) / 10) : 0;
                    NiXie_Value[6] = speed % 10;
                }
                else
                {
                    NiXie_Value[6] = ((speed % 100) / 10) ? ((speed % 100) / 10) : 16;
                    NiXie_Value[6] = speed % 10;
                }
            }
        }
        else if (factory_state == 2)
        {
            NiXie_Value[6] = dac_limit / 10;
            NiXie_Value[7] = dac_limit % 10;
            NiXie_Dot[6] = 1;
        }
    }
}

void Led_pro()
{
    if (distance_val < 10)
        dac_out = (dac_limit / 5) * 255;
    else if (distance_val > 90)
        dac_out = 255;
    else
        dac_out = ((dac_limit + (5 - dac_limit) / 80 * (distance_val - 10)) / 5) * 255;
    dac_output(dac_out);
    if (state == 0)
    {
        Led_Value[0] = (distance_val & 0x01);
        Led_Value[1] = (distance_val & 0x02);
        Led_Value[2] = (distance_val & 0x04);
        Led_Value[3] = (distance_val & 0x08);
        Led_Value[4] = (distance_val & 0x10);
        Led_Value[5] = (distance_val & 0x20);
        Led_Value[6] = (distance_val & 0x40);
        Led_Value[7] = (distance_val & 0x80);
    }
    else if (state == 1)
    {
        Led_Value[7] = 1;
    }
    else if (state == 2)
    {
        Led_Value[0] = Led_light;
    }
}

void Timer0_Isr(void) interrupt 1
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_Slow_Down == 200)
        NiXie_Slow_Down = 0;
    if (++NiXie_Location == 8)
        NiXie_Location = 0;
    if (++time_500ms == 500)
    {
        time_500ms = 0;
        // distance_val = Measure_Distance();
    }
    if (state == 2)
    {
        if (++time_100ms == 100)
        {
            time_100ms = 0;
            Led_light = ~Led_light;
        }
    }
    NiXie_Display(NiXie_Location, NiXie_Value[NiXie_Location], NiXie_Dot[NiXie_Location]);
    LED_Display(NiXie_Location, Led_Value[NiXie_Location]);
    Relay(Relay_flag);
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
    EA = 1;       //
}

void main()
{
    temp_val = Read_Temperature();
    while (temp_val == 850)
    {
        temp_val = Read_Temperature();
    }
    Init_Sys();
    Init_State();
    Timer0_Init();
    // Timer1_Init();
    while (1)
    {
        Key_pro();
        NiXie_pro();
        Led_pro();
    }
}