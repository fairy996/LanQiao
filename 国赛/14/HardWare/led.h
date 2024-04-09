#ifndef __LED_H_
#define __LED_H_

#include "sys.h"

void LED_Display(unsigned char Location, unsigned char enable);
void Beep(unsigned char flag);
void Relay(unsigned char flag);
#endif