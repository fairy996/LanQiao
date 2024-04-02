#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "led.h"
#include "nixie.h"
#include "key.h"
#include "iic.h"
#include "ultrasonic.h"
typedef unsigned char uchar;
typedef unsigned int uint;

void Delay12us(void); //@12.000MHz
void InitHC138(unsigned char channel);
void Init_Sys();
#endif