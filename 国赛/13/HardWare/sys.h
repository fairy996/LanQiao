#ifndef __SYS_H
#define __SYS_H

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "led.h"
#include "key.h"
#include "nixie.h"
#include "iic.h"
#include "ultrasonic.h"

void Init_HC138(unsigned char channel);
void Init_Sys();
#endif