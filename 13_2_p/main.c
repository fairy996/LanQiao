#include "sys.h"

unsigned char nixie_data[8] = {20, 20, 20, 20, 20, 20, 20, 20};
unsigned char nixie_pos[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char led_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char nixie_state = 0;
unsigned char v_max = 45;
unsigned char v_min = 5;
unsigned char led_lightflag = 0;
unsigned char para_state = 0;
unsigned char time_100ms = 0;
unsigned char distance_flag = 0;
unsigned int distance = 0;
unsigned char dac_digital = 0;
unsigned char para_flag = 1;
unsigned int time_500ms = 0;
void key_pro()
{
    unsigned char key_value = Key_Scan1();
    switch (key_value)
    {
    case 1: // S7
        if (nixie_state == 2)
        {
            if (para_state)
            {
                if ((v_min -= 5) < 5)
                    v_min = 50;
            }
            else
            {
                if ((v_max -= 5) < 5)
                    v_max = 50;
            }
        }
        break;
    case 2: // S6
        if (nixie_state == 2)
        {
            if (para_state)
            {
                if ((v_min += 5) > 50)
                    v_min = 5;
            }
            else
            {
                if ((v_max += 5) > 50)
                    v_max = 5;
            }
        }
        break;
    case 3: // S5
        para_state ^= 1;
        break;
    case 4: // S4
        if (nixie_state == 0)
            nixie_state = 1;
        else if (nixie_state == 1)
        {
            nixie_state = 2;
            para_state = 0;
            para_flag = 0;
        }
        else if (nixie_state == 2)
        {
            nixie_state = 0;
            para_flag = 1;
        }
        break;
    }
}
void Clear_dat()
{
    unsigned char i = 0;
    for (i = 0; i < 8; i++)
    {
        nixie_data[i] = 20;
        nixie_pos[i] = 0;
        led_data[i] = 0;
    }
}
void nixie_pro()
{
    unsigned char v_temporary = 0;
    unsigned int v_analog = 0;
    v_temporary = adc_read(0x03);
    v_analog = (v_temporary * 100) / 255 * 5;
    if (para_flag)
    {
        distance_flag = (v_analog > v_min * 10 && v_analog < v_max * 10);
        // if (distance_flag)
        //     distance = ultr_read();
    }
    led_lightflag = distance_flag;

    if (nixie_state == 0)
    {
        Clear_dat();
        nixie_data[0] = 16;
        nixie_data[5] = v_analog / 100;
        nixie_data[6] = v_analog % 100 / 10;
        nixie_data[7] = v_analog % 10;
        nixie_pos[5] = 1;
    }
    else if (nixie_state == 2)
    {
        Clear_dat();
        nixie_data[0] = 17;
        nixie_data[3] = v_max / 10;
        nixie_data[4] = v_max % 10;
        nixie_pos[3] = 1;
        nixie_data[6] = v_min / 10;
        nixie_data[7] = v_min % 10;
        nixie_pos[6] = 1;
    }
    else if (nixie_state == 1)
    {
        Clear_dat();
        nixie_data[0] = 19;
        if (distance_flag == 0)
        {
            nixie_data[5] = 10;
            nixie_data[6] = 10;
            nixie_data[7] = 10;
        }
        else
        {
            if (distance > 99)
            {
                nixie_data[5] = distance / 100;
                nixie_data[6] = distance % 100 / 10;
                nixie_data[7] = distance % 10;
            }
            else if (distance > 9)
            {
                nixie_data[5] = 20;
                nixie_data[6] = distance / 10;
                nixie_data[7] = distance % 10;
            }
            else
            {
                nixie_data[5] = 20;
                nixie_data[6] = 20;
                nixie_data[7] = distance;
            }
        }
    }
    NiXie_Control(nixie_data, nixie_pos);
}
void led_pro()
{
    led_data[0] = (nixie_state == 0);
    led_data[1] = (nixie_state == 1);
    led_data[2] = (nixie_state == 2);
    led_data[7] = (distance_flag) ? led_lightflag : 0;
    Led_control(led_data);
}
void Timer1_Isr(void) interrupt 3
{
    if (++time_100ms == 150)
    {
        time_100ms = 0;
        led_lightflag ^= 1;
    }
    if(distance_flag)
    {
        if(++time_500ms > 500)
        {
            time_500ms = 0;
            distance = ultr_read();
        }
    }
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
    EA = 1;       // 使能总中断
}
void Timer0_Init(void)
{
    AUXR &= 0xBF; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0;
    TH0 = 0;
    TF0 = 0;
    TR0 = 0;
    ET0 = 0;
}
void main(void)
{
    Init_Sys();
    Timer1_Init();
    Timer0_Init();
   
    while (1)
    {
        key_pro();
        led_pro();
        nixie_pro();
    }
}