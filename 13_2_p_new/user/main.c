#include "sys.h"

/***************参数设置*******************/
// 按键
uchar Key_Slow_Down;
uchar Key_Val, Key_Old, Key_Up, Key_Down;
// 数码管
uint NiXie_SLow_Down;
uchar NiXie_location;
uchar NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
uchar NiXie_Point[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED相关
uchar LED[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// 时间相关参数
uchar time_100ms;
uchar time_500ms;
// 界面相关参数
uchar state = 0; // 默认为电压界面0 1 测距界面 2 参数界面
// 数据内容
uchar V_UpLimit = 45;
uchar V_DownLimit = 5;
uint distance;
uchar V_Analog;
uint V_Digital;
uchar dac_out;
// 标志位
uchar LED_Light_Flag = 0;
uchar Distance_Flag = 0;
uchar limit_flag = 0; // 上电时为参数上限
uchar test_flag;
void NiXie_Clear()
{
    uchar i;
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

    Key_Val = Key_Scan_freestanding();
    Key_Down = Key_Val & (Key_Old ^ Key_Val);
    Key_Up = ~Key_Val & (Key_Old ^ Key_Val);
    Key_Old = Key_Val;

    switch (Key_Down)
    {
    case 4:
        NiXie_Clear();
        if (state == 0)
        {
            state = 1;
        }
        else if (state == 1)
        {
            state = 2;
            limit_flag = 0;
            Distance_Flag = 0;
        }
        else if (state == 2)
        {
            state = 0;
        }
        break;
    case 5:
        if (state == 2)
        {
            NiXie_Clear();
            limit_flag = ~limit_flag;
        }
        break;
    case 6:
        if (state == 2)
        {
            NiXie_Clear();
            if (limit_flag == 0)
            {
                V_UpLimit += 5;
                if (V_UpLimit > 50)
                    V_UpLimit = 5;
            }
            else
            {
                V_DownLimit += 5;
                if (V_DownLimit > 50)
                    V_DownLimit = 5;
            }
        }
        break;
    case 7:
        if (state == 2)
        {
            NiXie_Clear();
            if (limit_flag == 0)
            {
                V_UpLimit -= 5;
                if (V_UpLimit < 5)
                    V_UpLimit = 50;
            }
            else
            {
                V_DownLimit -= 5;
                if (V_DownLimit < 5)
                    V_DownLimit = 50;
            }
        }
        break;
    }
}

void NiXie_pro()
{
    if (NiXie_SLow_Down)
        return;
    NiXie_SLow_Down = 1;
    V_Analog = adc_read(0x03);
    V_Digital = (V_Analog * 100) / 255 * 5;
    distance = measure_distance();
    if (state != 2)
    {
        Distance_Flag = ((V_Digital > V_DownLimit * 10) && (V_Digital < V_UpLimit * 10));
    }
    if (state == 0)
    {
        NiXie_Value[0] = 17;
        NiXie_Value[5] = V_Digital / 100;
        NiXie_Value[6] = V_Digital % 100 / 10;
        NiXie_Value[7] = V_Digital % 10;
        NiXie_Point[5] = 1;
    }
    else if (state == 1)
    {
        NiXie_Value[0] = 19;
        if (Distance_Flag)
        {
            if (distance > 100)
            {
                NiXie_Value[5] = distance / 100;
                NiXie_Value[6] = distance % 100 / 10;
                NiXie_Value[7] = distance % 10;
            }
            else if (distance > 10)
            {
                NiXie_Value[6] = distance / 10;
                NiXie_Value[7] = distance % 10;
            }
            else if (distance > 0)
                NiXie_Value[7] = distance;
        }
        else
        {
            NiXie_Value[5] = 10;
            NiXie_Value[6] = 10;
            NiXie_Value[7] = 10;
        }
    }
    else if (state == 2)
    {
        NiXie_Value[0] = 20;
        NiXie_Value[3] = V_UpLimit / 10;
        NiXie_Value[4] = V_UpLimit % 10;
        NiXie_Value[6] = V_DownLimit / 10;
        NiXie_Value[7] = V_DownLimit % 10;
        NiXie_Point[3] = 1;
        NiXie_Point[6] = 1;
    }
}

void Led_pro()
{
    LED[0] = (state == 0);
    LED[1] = (state == 1);
    LED[2] = (state == 2);
    LED[7] = (Distance_Flag) ? LED_Light_Flag : 0;
}

void DAC_pro()
{
    unsigned char dac_out_analog;
    if (Distance_Flag)
    {
        if (distance > 20)
            dac_out_analog = 1 + (1 / 15) * (distance - 20);
        else if (distance > 80)
            dac_out_analog = 5;
        else
            dac_out_analog = 1;
        dac_out = (dac_out_analog / 5) * 255;
        dac_export(dac_out);
    }
}

void Timer0_Isr(void) interrupt 1
{
    if (++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if (++NiXie_SLow_Down == 500)
        NiXie_SLow_Down = 0;
    if (++NiXie_location == 8)
        NiXie_location = 0;
    if (++time_100ms == 100)
    {
        time_100ms = 0;
        LED_Light_Flag = ~LED_Light_Flag;
    }
    NiXie_Display(NiXie_location, NiXie_Value[NiXie_location], NiXie_Point[NiXie_location]);
    Led_Display(NiXie_location, LED[NiXie_location]);
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
    EA = 1;        // 使能总中断
}

void main()
{
    Init_Sys();
    Timer0_Init();
    while (1)
    {
        Key_pro();
        Led_pro();
        DAC_pro();
        NiXie_pro();
    }
}