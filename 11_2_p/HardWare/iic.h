#ifndef __IIC_H_
#define __IIC_H_

#include "sys.h"

void AT24C02_Write(unsigned char addr, unsigned char dat);
unsigned char AT24C02_Read(unsigned char addr);
unsigned char adc_read(unsigned char addr);
#endif