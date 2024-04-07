#include "nixie.h"

unsigned char Seg_Table[] =
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
        0xFF, //全灭  16
        0x8C, //P    17
        0xC1, //U    18
        0xAB  //N    19
};

unsigned char NiXie_Pos[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};


void NiXie_Display(unsigned char Location,unsigned char Value,unsigned char point)
{
    P0 = 0xFF;
    Init_HC138(7);

    P0 = 0x01 << Location;
    Init_HC138(6);

    P0 = Seg_Table[Value];
    if(point)
        P0 = P0 & 0x7F;
    Init_HC138(7);
}