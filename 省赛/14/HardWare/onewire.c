/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(外部晶振12MHz) STC89C52RC单片机
  日    期: 2011-8-9
*/
#include "sys.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	t *= 12;
	while(t--);
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat & 0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20设备初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

unsigned int Access_Temp(unsigned char dot)
{
	uint temp;
	uchar HSB,LSB;
	init_ds18b20();
	Write_DS18B20(0xCC);  //跳过ROM命令
	Write_DS18B20(0x44);  //转换温度命令
	init_ds18b20();
	Write_DS18B20(0xCC);  //跳过ROM命令
	Write_DS18B20(0xBE);  //读取温度值命令
	LSB = Read_DS18B20();   //读取温度值低字节
	HSB = Read_DS18B20();   //读取温度值高字节
	temp = (HSB << 8) | LSB;

	switch(dot)
	{
		case 0:temp = temp * 0.0625;break;
		case 1:temp = temp * 0.625;break;
		case 2:temp = temp * 6.25;break;
	}
	return temp;
}





