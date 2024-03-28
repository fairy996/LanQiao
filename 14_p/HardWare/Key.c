#include "key.h"

unsigned char key_up = 0;
unsigned char flag = 0;
sbit L1 = P4^4;
sbit L2 = P4^2;
sbit L3 = P3^5;
sbit L4 = P3^4;
sbit H4 = P3^3;
sbit H3 = P3^2;
sbit H2 = P3^1;
sbit H1 = P3^0;
unsigned char key_scan(void)
{
    uchar key_num;
    L1 = L2 = L3 = L4 = 1;
    H4 = H3 = H2 = H1 = 0;
    if((key_up == 1) && (L1 == 0 || L2 == 0 || L3 == 0 || L4 == 0))
    {
        Delayms(20);
        if(L1 == 0)  key_num = 1;
        else if(L2 == 0) key_num = 2;
        else if(L3 == 0) key_num = 3;
        else if(L4 == 0) key_num = 4;
        else return 0;
        key_up = 0;
        

        L1 = L2 = L3 = L4 = 0;
        H4 = H3 = H2 = H1 = 1;
        if(H1 == 0)  {flag = 1;return key_num;}
        else if(H2 == 0) return key_num + 4;
        else if(H3 == 0) return key_num + 8;
        else if(H4 == 0) return key_num + 12;
    }
    else if(L1 == 1 && L2 == 1 && L3 == 1 && L4 == 1)
    {
        key_up = 1;
        if(flag == 1)
        {
            flag = 0;
            return 25;
        }
        
    }
    return 0; // 没有按键按下
}