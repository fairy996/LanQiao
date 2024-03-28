#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "led.h"
#include "NiXie.h"
#include "key.h"
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"
typedef unsigned char uchar;
typedef unsigned int uint;

void InitHC138(unsigned char channel);
void Delayus(unsigned int us);	//@11.0592MHz
void Delayms(unsigned int ms);	//@11.0592MHz
void Init_Sys();
#endif