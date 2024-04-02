#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "led.h"
#include "key.h"
#include "nixie.h"
#include "iic.h"
#include "ultr.h"
#include "main.h"
extern unsigned char nixie_data[8];
extern unsigned char nixie_pos[8];
void Init_Hc138(unsigned char channel);
void Init_Sys(void);
void Delay_us(unsigned int us); //@12.000MHz
void Delay_ms(unsigned int ms); //@12.000MHz
#endif