#include"../inc/rp2a03.h"


RP2A03::RP2A03()
{

}


RP2A03::~RP2A03()
{

}

void RP2A03::fetch(void)
{

}


void RP2A03::decode(U8 byeCode)
{

}


void RP2A03::setFlag(U8 flags)
{
    if(this->status != NULL)
    {
        if(flags & carryFlag){ status->C = 0x1; }
        if(flags & zeroFlag){ status->Z = 0x1; }
        if(flags & interruptDisableFlag){ status->I = 0x1; }
        if(flags & decimalModeFlag){ status->D = 0x1; }
        if(flags & breakFlag){ status->B = 0x1; }
        if(flags & unusedFlag){ status->UNUSED = 0x1; }
        if(flags & overflowFlag){ status->V = 0x1; }
        if(flags & negativeFlag){ status->N = 0x1; }
    }
    else
    {
        // Consider throwing an exception when status pointer is NULL
    }
}