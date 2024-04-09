#include "sys.h"

//*************变量声明区***************
//按键
unsigned int Key_Slow_Down;
unsigned char Key_Val;
unsigned char Key_Up;
unsigned char Key_Down;
unsigned char Key_Old;
//数码管
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Location;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Dot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//LED
unsigned char Led_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//时间
unsigned int Time_5s;
//测量数据
unsigned int Voltiage_para;
unsigned char Digital_V;
unsigned char Analog_V;
unsigned char Analog_V_Old;
unsigned char Error_Count = 0;
//标志位
unsigned char state = 0;//0 数据界面 1 参数界面 2计数界面
unsigned char Count = 0;//电压相关计数值
unsigned char Time_5s_Flag = 0;
unsigned char Count_Flag = 0;
//*****************函数*********************
void Clear_NiXie()
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        NiXie_Value[i] = 16;
        NiXie_Dot[i] = 0;
    }
}

void Key_pro()
{
    if(Key_Slow_Down)
        return;
    Key_Slow_Down = 1;

    Key_Val = Key_Scan();
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
    Key_Old = Key_Val;

    switch(Key_Down)
    {
    case 12:
        Clear_NiXie();
        Error_Count = 0;
        if(state == 0)
            state = 1;
        else if(state == 1)
        {
            state = 2;
            AT24C02_Write(0x01, Voltiage_para / 10);
					//AT24C02_Write(0x01, 0);
        }
        else if(state == 2)
            state = 0;
        break;
    case 13:
        Clear_NiXie();
        Error_Count = 0;
        if(state == 2)
        {
            Count = 0;
        }
        break;
    case 16:
        Clear_NiXie();
        Error_Count = 0;
        if(state == 1)
        {
            Voltiage_para += 50;
            if(Voltiage_para > 500)
                Voltiage_para = 0;
        }
        break;
    case 17:
        Clear_NiXie();
        Error_Count = 0;
        if(state == 1)
        {
            Voltiage_para -= 50;
            if(Voltiage_para < 0)
                Voltiage_para = 500;
        }

        break;
    case 25:
        //Clear_NiXie();
        Error_Count++;
        break;
    }
}

void NiXie_pro()
{
    if(NiXie_Slow_Down)
        return;
    NiXie_Slow_Down = 1;
//    Analog_V_Old = Analog_V;
    Digital_V = adc_read(0x03);
    Analog_V = (Digital_V * 500) / 255;
//    if(Analog_V_Old > Voltiage_para && Analog_V < Voltiage_para)
//        Count++;
		if(Analog_V > Voltiage_para)
		{
			Count_Flag = 1;
		}
		if((Count_Flag == 1) && (Analog_V < Voltiage_para))
		{
			Count++;
			Count_Flag = 0;
		}
    if(Analog_V < Voltiage_para)
        Time_5s_Flag = 1;
    else
        Time_5s_Flag = 0;
    if(state == 0)
    {
        NiXie_Value[0] = 18;
        NiXie_Value[5] = Analog_V / 100;
        NiXie_Value[6] = (Analog_V % 100) / 10;
        NiXie_Value[7] = Analog_V % 10;
        NiXie_Dot[5] = 1;
    }
    else if(state == 1)
    {
        NiXie_Value[0] = 17;
        NiXie_Value[5] = Voltiage_para / 100;
        NiXie_Value[6] = (Voltiage_para % 100) / 10;
        NiXie_Value[7] = Voltiage_para % 10;
        NiXie_Dot[5] = 1;
    }
    else if(state == 2)
    {
        NiXie_Value[0] = 19;
        NiXie_Value[1] = (Count / 1000000) ? (Count / 1000000) : 16;
        NiXie_Value[2] = ((Count % 1000000) / 100000) ? ((Count % 1000000) / 100000) : 16;
        NiXie_Value[3] = ((Count % 100000) / 10000) ? ((Count % 100000) / 10000) : 16;
        NiXie_Value[4] = ((Count % 10000) / 1000) ? ((Count % 10000) / 1000) : 16;
        NiXie_Value[5] = ((Count % 1000) / 100) ? ((Count % 1000) / 100) : 16;
        NiXie_Value[6] = ((Count % 100) / 10) ? ((Count % 100) / 10) : 16;
        NiXie_Value[7] = (Count % 10) ? (Count % 10) : 16;
    }
}
void Led_pro()
{
    Led_Value[0] = (Time_5s == 5001);
    Led_Value[1] = (Count % 2);
	Led_Value[2] = (Error_Count >= 3) ? 1 : 0;
}
void Timer0_Isr(void) interrupt 1
{
    if(++Key_Slow_Down == 10)
        Key_Slow_Down = 0;
    if(++NiXie_Slow_Down == 200)
        NiXie_Slow_Down = 0;
    if(++NiXie_Location == 8)
        NiXie_Location = 0;
    	if(Time_5s_Flag)
    	{
    		if(++Time_5s >= 5000)
    			Time_5s = 5001;
    	}
    	else
    		Time_5s = 0;
    NiXie_Display(NiXie_Location, NiXie_Value[NiXie_Location], NiXie_Dot[NiXie_Location]);
    LED_Display(NiXie_Location, Led_Value[NiXie_Location]);
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
    AUXR &= 0x7F;			//定时器时钟12T模式
    TMOD &= 0xF0;			//设置定时器模式
    TL0 = 0x18;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    TR0 = 1;				//定时器0开始计时
    ET0 = 1;				//使能定时器0中断
    EA = 1;
}

void main()
{
    Init_Sys();
    Timer0_Init();
    Voltiage_para = AT24C02_Read(0x01) * 10;
    while(1)
    {
        Key_pro();
        NiXie_pro();
        Led_pro();
    }
}
