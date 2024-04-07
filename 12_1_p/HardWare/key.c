#include "key.h"

sbit C1 = P4 ^ 4;
sbit C2 = P4 ^ 2;
sbit C3 = P3 ^ 5;
sbit C4 = P3 ^ 4;

sbit L1 = P3 ^ 0;
sbit L2 = P3 ^ 1;
sbit L3 = P3 ^ 2;
sbit L4 = P3 ^ 3;

unsigned char Key_Scan()
{
    unsigned char temp = 0;
    C1 = 0;
    C2 = C3 = C4 = 1;
    L1 = L2 = L3 = L4 = 1;
    if (L1 == 0)
        temp = 7;
    if (L2 == 0)
        temp = 6;
    if (L3 == 0)
        temp = 5;
    if (L4 == 0)
        temp = 4;
    C1 = 0;
    C2 = C3 = C4 = 1;
    L1 = L2 = L3 = L4 = 1;
    if (L1 == 0)
        temp = 7;
    if (L2 == 0)
        temp = 6;
    if (L3 == 0)
        temp = 5;
    if (L4 == 0)
        temp = 4;
    C2 = 0;
    C1 = C3 = C4 = 1;
    L1 = L2 = L3 = L4 = 1;
    if (L1 == 0)
        temp = 11;
    if (L2 == 0)
        temp = 10;
    if (L3 == 0)
        temp = 9;
    if (L4 == 0)
        temp = 8;
    C3 = 0;
    C2 = C1 = C4 = 1;
    L1 = L2 = L3 = L4 = 1;
    if (L1 == 0)
        temp = 15;
    if (L2 == 0)
        temp = 14;
    if (L3 == 0)
        temp = 13;
    if (L4 == 0)
        temp = 12;
    C4 = 0;
    C2 = C1 = C3 = 1;
    L1 = L2 = L3 = L4 = 1;
    if (L1 == 0)
        temp = 19;
    if (L2 == 0)
        temp = 18;
    if (L3 == 0)
        temp = 17;
    if (L4 == 0)
        temp = 16;
    return temp;
}