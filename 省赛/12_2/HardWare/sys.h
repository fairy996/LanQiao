#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "led.h"
#include "nixie.h"
#include "key.h"
#include "iic.h"

void Init_HC138(unsigned char channel);
void Init_Sys();
#endif