#include "nixie.h"

code unsigned char Seg_Table[] =
    {
        0xc0, // 0  --0
        0xf9, // 1
        0xa4, // 2
        0xb0, // 3
        0x99, // 4
        0x92, // 5
        0x82, // 6
        0xf8, // 7
        0x80, // 8
        0x90, // 9
        0x88, // A  --10
        0x83, // b  --11
        0xc6, // C  --12
        0xa1, // d  --13
        0x86, // E  --14
        0x8e, // F  --15
        0xC1, // U  --16
        0x8C, // P  --17
        0xBF, //-  --18
        0xC3, // L  --19
        0xFF  // 全灭  --20
};
void NiXie_Bit(unsigned char poistion, unsigned char value, unsigned char pos)
{
    P0 = 0xFF;
    Init_Hc138(7);
    P0 = 0x01 << (poistion - 1);
    Init_Hc138(6);
    P0 = Seg_Table[value];
    if (pos)
        P0 &= 0x7F;
    Init_Hc138(7);
    Delay_ms(1);
}

void NiXie_Control(unsigned char *NiXie, unsigned char *pos)
{
    NiXie_Bit(1, NiXie[0], pos[0]);
    NiXie_Bit(2, NiXie[1], pos[1]);
    NiXie_Bit(3, NiXie[2], pos[2]);
    NiXie_Bit(4, NiXie[3], pos[3]);
    NiXie_Bit(5, NiXie[4], pos[4]);
    NiXie_Bit(6, NiXie[5], pos[5]);
    NiXie_Bit(7, NiXie[6], pos[6]);
    NiXie_Bit(8, NiXie[7], pos[7]);
}