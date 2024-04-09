#ifndef __DS1302_H_
#define __DS1302_H_

#include "sys.h"

void Write_Ds1302(unsigned  char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat ) ;
unsigned char Read_Ds1302_Byte ( unsigned char address );

void DS1302_Time_Write(unsigned char *rt);
void DS1302_Time_Read(unsigned char *rt);
#endif