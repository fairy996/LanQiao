#include "nixie.h"

code unsigned char Seg_Table[] = 
{
0xc0, //0  --0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0x88, //A  --10
0x83, //b  --11
0xc6, //C  --12
0xa1, //d  --13
0x86, //E  --14
0x8e, //F  --15
0x89, //H  --16
0x8C, //P  --17
0xBF, //-  --18
0xFF  //全灭  --19
};

void NiXie_Bit(unsigned char location,unsigned char value,unsigned char pos)
{
    P0 = 0xFF;
    InitHC138(7);
    P0 = 0x01 << (location);
    InitHC138(6);
    if(pos == 0)
    {
        P0 = Seg_Table[value];
    }
    else
    {
        P0 = Seg_Table[value] & 0x7F;
    }
    InitHC138(7);
    Delayms(1);
}

void NiXie_Show(unsigned char *NiXie,unsigned char *pos)
{
    NiXie_Bit(0,NiXie[0],pos[0]);
    NiXie_Bit(1,NiXie[1],pos[1]);
    NiXie_Bit(2,NiXie[2],pos[2]);
    NiXie_Bit(3,NiXie[3],pos[3]);
    NiXie_Bit(4,NiXie[4],pos[4]);
    NiXie_Bit(5,NiXie[5],pos[5]);
    NiXie_Bit(6,NiXie[6],pos[6]);
    NiXie_Bit(7,NiXie[7],pos[7]);
    NiXie_Bit(8,0,0);
}