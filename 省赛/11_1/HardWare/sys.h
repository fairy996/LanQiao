#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "key.h"
#include "led.h"
#include "nixie.h"
#include "iic.h"
#include "onewire.h"


void Init_HC138(unsigned char channel);
void Init_Sys();
#endif