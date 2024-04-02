#ifndef __SYS_H_
#define __SYS_H_

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "led.h"
#include "key.h"
#include "nixie.h"
#include "onewire.h"
#include "ds1302.h"

void Init_Sys();
void Init_HC138(unsigned char channel);
#endif