#ifndef _IIC_H
#define _IIC_H

#include "sys.h"
void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 


unsigned char adc_read(unsigned char addr);
//void dac_out(unsigned char date);
#endif