#ifndef __DS1302_H_
#define __DS1302_H_

#include "sys.h"

void DS1302_Time_Write(unsigned char *rt);
void DS1302_Time_Read(unsigned char *rt);
#endif