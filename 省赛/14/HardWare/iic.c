/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include "iic.h"

#define DELAY_TIME 5

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//总线启动条件
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0：应答，1：非应答
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
			IIC_Delay(DELAY_TIME);
			da <<= 1;
			if(SDA) da |= 1;
			SCL = 0;
			IIC_Delay(DELAY_TIME);
    }
    return da;    
}



void dac_out(unsigned char date)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_Stop();
}


//ADC采集结果 0x01采集光敏电阻 0x03采集滑动变阻器
unsigned char adc_read(unsigned char addr)
{
    uchar temp;
	IIC_Start();//IIC总线起始信号										
	IIC_SendByte(0x90);//PCF8591的写设备地址	 				
	IIC_WaitAck();//等待从机应答	  							
	IIC_SendByte(addr);//发送PCF8591的控制字节；选择选择模拟量输入通道；光敏传感器相应的控制寄存器应写入0x01	 				
	IIC_WaitAck();//等待从机应答	  							
	IIC_Stop();	//IIC总线停止信号 								
	
	
	IIC_Start();//IIC总线起始信号									
	IIC_SendByte(0x91); //PCF8591的读设备地址						
	IIC_WaitAck(); //等待从机应答								
	temp = IIC_RecByte();//读取PCF8591通道1的数据 	 			
	IIC_SendAck(1); //产生非应答信号									
	IIC_Stop(); //IIC总线停止信号										
	return temp;
}