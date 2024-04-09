#include "sys.h"

//************************参数定义*******************************
uchar NiXie[8] = {19,19,19,19,19,19,19,19};
uchar pos[8] = {0,0,0,0,0,0,0,0};
uchar led[8] = {0,0,0,0,0,0,0,0};

uchar state1 = 0,state1_old = 0; //默认为时间界面 0 时间界面 1回显界面 2参数界面 3温湿度界面
uchar state2 = 0; //默认为温度回显界面 0 温度回显 1湿度回显 2 时间回显
uchar trigger_count = 0;//温度采集次数
uchar temp_para = 30; //温度参数
uchar rt_w[3] = {23,59,50};
uchar re_r[3];
uint temp_max = 0;//最高温度
uint temp_ave = 0;//平均温度
uint temp = 0,temp_last = 0;//获取温度值
uchar hum_max = 0,hum_ave = 0,hum = 0,hum_last = 0;//最高湿度 平均湿度 最低湿度
uchar access_h = 0;access_m = 0;//最近一次采集温度和湿度的时间
uchar Rd = 0,Rd_last = 0;//本次获取光敏电压和上次获取光敏电压
uchar trigger_flag = 0;//采集触发标志 0 未触发 1 触发
uint time_3s,time_1s;//定时器3s相关参数
uint freq;
uchar data_flag = 0;//数据是否有效
uchar Waring_flag = 0;//是否报警
uchar data_up = 0;//是否温湿度都增加
//************************驱动代码*******************************
void key_pro()
{
    uchar key = key_scan();
    switch(key)
    {
        case 9://S5
            if(state1 == 1)
            {
                if(state2 == 0)
                {
                    state2 = 1; 
                }
                else if(state2 == 1)
                {
                    state2 = 2;
                }
                else if(state2 == 2)
                {
                    state2 = 0;
                }
            }
            break;
        case 10://S9

            break;
        case 13://S4
            if(state1 == 0)
            {
                state2 = 0;
                state1 = 1;
            }
            else if(state1 == 1)
            {
                state1 = 2;
            }
            else if(state1 == 2)
            {
                state1 = 0;
            }
            break;
        case 14://S8

            break;
        case 15:
            temp_ave = Access_Temp(1);
            break;
    }
}
void nixie_pro()
{
    if(state1 == 0)
    {
        DS1302_Time_Read(re_r);
        NiXie[2] = NiXie[5] = 18;
        NiXie[0] = re_r[0] / 10;
        NiXie[1] = re_r[0] % 10;
        NiXie[3] = re_r[1] / 10;
        NiXie[4] = re_r[1] % 10;
        NiXie[6] = re_r[2] / 10;
        NiXie[7] = re_r[2] % 10;
        pos[6] = 0;
    }
    else if(state1 == 1)
    {
        if(state2 == 0)
        {
            pos[6] = 1;
            NiXie[0] = 12;
            NiXie[1] = 19;
            NiXie[2] = temp_max / 100;
            NiXie[3] = (temp_max % 100) / 10;
            NiXie[4] = 18;
            NiXie[5] = temp_ave / 100;
            NiXie[6] = (temp_ave % 100) / 10;
            NiXie[7] = temp_ave % 10;
        }
        else if(state2 == 1)
        {
            pos[6] = 1;
            NiXie[0] = 16;
            NiXie[1] = 19;
            NiXie[2] = hum_max / 100;
            NiXie[3] = (hum_max % 100) / 10;
            NiXie[4] = 18;
            NiXie[5] = hum_ave / 100;
            NiXie[6] = (hum_ave % 100) / 10;
            NiXie[7] = hum_ave % 10;
        }
        else if(state2 == 2)
        {
            NiXie[0] = 15;
            NiXie[1] = trigger_count/10;
            NiXie[2] = trigger_count%10;
            NiXie[3] = access_h / 10;
            NiXie[4] = access_h % 10;
            NiXie[5] = 18;
            NiXie[6] = access_m / 10;
            NiXie[7] = access_m % 10;
            pos[6] = 0;
        }
    }
    else if(state1 == 2)
    {
        NiXie[0] = 17;
        NiXie[6] = temp_para / 10;
        NiXie[7] = temp_para % 10;
        NiXie[1] = NiXie[2] = NiXie[3] = NiXie[4] = NiXie[5] = 19;
        pos[6] = 0;
    }
    else if(state1 == 3)
    {
        NiXie[0] = 14;
        NiXie[5] = 18;
        NiXie[1] = NiXie[2] = 19;
        NiXie[3] = temp / 100;
        NiXie[4] = (temp % 100) / 10;
        if(data_flag == 0)
        {
            NiXie[6] = 10;
            NiXie[7] = 10;
        }
        else
        {
            NiXie[6] = hum / 10;
            NiXie[7] = hum % 10;
        }
        pos[6] = 0;
    }
    NiXie_Show(NiXie,pos);
}
void led_pro()
{
    switch(state1)
    {
        case 0:led[0] = 1;led[1] = 0;led[2] = 0;break;
        case 1:led[0] = 0;led[1] = 1;led[2] = 0;break;
        case 3:led[0] = 0;led[1] = 0;led[2] = 1;break;
    }
    if(Waring_flag == 1)
    {
        led[3] = 0;
        Delayms(100);
        led[3] = 1;
        Delayms(100);
    }
    else
        led[3] = 1;
    if(data_flag == 0)
        led[4] = 0;
    else
        led[4] = 1;
    if(data_up == 1)
        led[5] = 0;
    else
        led[5] = 0;
    led_control(led);
}
unsigned int Access_Hum()
{
    uint temp;
    freq *= 100;
    if (freq < 200 || freq > 2000)
        return 0;
    else
        temp = ((freq - 200) * 2 / 45 + 10) * 10;
        return temp;
}
void Access_Temp_Hum()
{
    Rd_last = Rd;
    Rd = adc_read(0xa1);
    if(Rd < 80 && Rd_last > 80 && trigger_flag == 0)
    {
        trigger_flag = 1;
        if(++trigger_count == 100)
        {
            trigger_count = 99;
        }
    }
    //被触发且触发时长达到3s
    else if(time_3s >= 3000 && trigger_flag == 1)
    {
        state1 = state1_old;
        trigger_flag = 0;
    }
    if(trigger_flag == 1)
    {
        state1_old = state1;
        state1 = 3;
        temp = Access_Temp(1);
        hum = Access_Hum();
        if(hum == 0)
        {
            trigger_flag--;
            data_flag = 0;
        }
        else
        {
            data_flag = 1; //数据有效
            Waring_flag = (temp > temp_para);//是否进行温度越界报警
            data_up = (trigger_count >= 2)&&(temp > temp_last) && (hum > hum_last);
            temp_max = (temp_max > temp) ? temp_max : temp;
            hum_max = (hum_max > hum) ? hum_max : hum;
            temp_ave = (temp_ave * (trigger_count - 1) + temp) / trigger_count;
            hum_ave = (hum_ave * (trigger_count - 1) + hum) / trigger_count;
        }
    }
}
void Init_Timer0()
{
    AUXR &= 0x7F; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式,不影响定时器1的使用
    TMOD |= 0x05; // 设置定时器模式
    TL0 = 0;      // 设置定时初始值
    TH0 = 0;      // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
}
void Timer1_Init(void) // 1����@12.000MHz
{
    AUXR &= 0xBF; 
    TMOD &= 0x0F; 
    TL1 = 0x18;  
    TH1 = 0xFC;  
    TF1 = 0;     
    TR1 = 1;      
    ET1 = 1;      
    EA = 1;      
}
void Service_Timer1() interrupt 3
{
   
    if(++time_1s == 10)
    {
        time_1s = 0;
        freq = (TH0 << 8) | TL0;
        TH0 = 0;
        TL0 = 0;
    }
    if(trigger_flag == 1)
    {
        if(++time_3s >= 3000)
        {
            
            time_3s = 3001;
        }
    }
    else
        time_3s = 0;
}
//************************主循环*********************************
void main(void)
{
    Init_Sys();
    Timer1_Init();
    Init_Timer0();
    DS1302_Time_Write(rt_w);
    temp = Access_Temp(1);
    adc_read(0xa1);
    while(1)
    {
        Access_Temp_Hum();
        key_pro();
        nixie_pro();
        led_pro();
    }
}
