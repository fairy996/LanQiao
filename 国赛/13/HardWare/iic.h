#ifndef __IIC_H_
#define __IIC_H_

#include "sys.h"

unsigned char adc_read(unsigned char addr);
void Write_AT24C02(unsigned char addr, unsigned char dat);
void dac_output(unsigned char dat);
#endif