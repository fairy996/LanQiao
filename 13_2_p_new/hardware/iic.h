#ifndef __IIC_H_
#define __IIC_H_

#include "sys.h"

unsigned char adc_read(unsigned char addr);
void dac_export(unsigned char temp);
#endif