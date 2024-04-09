#include "sys.h"

//******************��������*************************
unsigned int Key_Slow_Down;
unsigned char Key_Val;
unsigned char Key_Down;
unsigned char Key_Up;
unsigned char Key_Old;
// �����
unsigned int NiXie_Slow_Down;
unsigned char NiXie_Location;
unsigned char NiXie_Value[8] = {16, 16, 16, 16, 16, 16, 16, 16};
unsigned char NiXie_Dot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// LED
unsigned char Led_Value[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// ʱ�����

// ����
unsigned int temp;
unsigned char temp_max_val = 30;
unsigned char temp_min_val = 20;
unsigned char temp_max_old;
unsigned char temp_min_old;
unsigned char voltiage;
// ��־λ
unsigned char state = 0;	 // 0 ���ݽ��� 1��������
unsigned char Para_Flag = 0; // 0 ��������Ҫ�� 1 ����������Ҫ��
unsigned char Para_Mode = 0; // 0���� 1����
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

	Key_Val = Key_Scan1();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	Key_Old = Key_Val;

	switch (Key_Down)
	{
	case 4:
		Clear_NiXie();
		if (state == 0)
		{
			state = 1;
			temp_max_old = temp_max_val;
			temp_min_old = temp_min_val;
			Para_Mode = 0;
		}
		else if (state == 1)
		{
			state = 0;
			if (temp_max_val < temp_min_val)
			{
				temp_max_val = temp_max_old;
				temp_min_val = temp_min_old;
				Para_Flag = 1;
			}
			else
			{
				Para_Flag = 0;
			}
		}
		break;
	case 5:
		Clear_NiXie();
		if(state == 1)
			Para_Mode = ~Para_Mode;
		break;
	case 6:
		Clear_NiXie();
		if (state == 1)
		{
			if (Para_Mode)
			{
				if (++temp_max_val == 99)
					temp_max_val = 99;
			}
			else
			{
				if (++temp_min_val == 99)
					temp_min_val = 99;
			}
		}
		break;
	case 7:
		Clear_NiXie();
		if (state == 1)
		{
			if (Para_Mode)
			{
				if (--temp_max_val == 0)
					temp_max_val = 0;
			}
			else
			{
				if (--temp_min_val == 0)
					temp_min_val = 0;
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

	temp = Read_Temp() / 10;

	if (state == 0)
	{
		NiXie_Value[0] = 12;
		NiXie_Value[6] = temp / 10;
		NiXie_Value[7] = temp % 10;
	}
	else if (state == 1)
	{
		NiXie_Value[0] = 17;
		NiXie_Value[3] = temp_max_val / 10;
		NiXie_Value[4] = temp_max_val % 10;
		NiXie_Value[6] = temp_min_val / 10;
		NiXie_Value[7] = temp_min_val % 10;
	}
}

void Led_pro()
{
	if (temp > temp_max_val)
		voltiage = (4 / 5) * 255;
	else if (temp < temp_min_val)
		voltiage = (2 / 5) * 255;
	else
		voltiage = (3 / 5) * 255;
	dac_out(voltiage);
	Led_Value[0] = (temp > temp_max_val);
	Led_Value[1] = ((temp <= temp_max_val) && (temp >= temp_min_val));
	Led_Value[2] = (temp < temp_min_val);
	Led_Value[3] = Para_Flag;
}
void Timer0_Isr(void) interrupt 1
{
	if (++Key_Slow_Down == 10)
		Key_Slow_Down = 0;
	if (++NiXie_Slow_Down == 100)
		Key_Slow_Down = 0;
	if (++NiXie_Location == 8)
		NiXie_Location = 0;

	NiXie_Display(NiXie_Location, NiXie_Value[NiXie_Location], NiXie_Dot[NiXie_Location]);
	LED_Display(NiXie_Location, Led_Value[NiXie_Location]);
}

void Timer0_Init(void) // 1����@12.000MHz
{
	AUXR &= 0x7F; // ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0; // ���ö�ʱ��ģʽ
	TL0 = 0x18;	  // ���ö�ʱ��ʼֵ
	TH0 = 0xFC;	  // ���ö�ʱ��ʼֵ
	TF0 = 0;	  // ���TF0��־
	TR0 = 1;	  // ��ʱ��0��ʼ��ʱ
	ET0 = 1;	  // ʹ�ܶ�ʱ��0�ж�
	EA = 1;
}

void main()
{
	temp = Read_Temp();
	while (temp == 850)
		temp = Read_Temp();
	Init_Sys();
	Timer0_Init();

	while (1)
	{
		Key_pro();
		NiXie_pro();
		Led_pro();
	}
}
