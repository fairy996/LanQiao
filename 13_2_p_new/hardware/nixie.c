#include "nixie.h"

uchar Seg_Table[] =
    {
        0xc0, // 0
        0xf9, // 1
        0xa4, // 2
        0xb0, // 3
        0x99, // 4
        0x92, // 5
        0x82, // 6
        0xf8, // 7
        0x80, // 8
        0x90, // 9
        0x88, // A
        0x83, // b
        0xc6, // C
        0xa1, // d
        0x86, // E
        0x8e, // F
        0xFF, // 灭 16
        0xC1, // U 17
        0xBF, //-  18
        0xC3, // L 19
        0x8C  // P 20
};
uchar NiXie_Bit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

void NiXie_Display(unsigned char location, unsigned char value, unsigned char point)
{
    P0 = 0xFF;
    InitHC138(7);

    P0 = NiXie_Bit[location];
    InitHC138(6);

    P0 = Seg_Table[value];
    if (point == 1)
        P0 = P0 & 0x7F;
    InitHC138(7);
}  