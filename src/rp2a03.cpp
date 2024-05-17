#include"../inc/rp2a03.h"


RP2A03::RP2A03(){}
RP2A03::~RP2A03(){}

void RP2A03::fetch(void){}
void RP2A03::decode(U8 byeCode){}

void RP2A03::reset(){}
void RP2A03::NMI(){}
void RP2A03::IRQ(){}


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


/************************************
 *          Instructions            *
 ************************************/

void RP2A03::LDA(){}
void RP2A03::LDX(){}
void RP2A03::LDY(){}
void RP2A03::STA(){}
void RP2A03::STX(){}
void RP2A03::STY(){}
void RP2A03::TAX(){}
void RP2A03::TAY(){}
void RP2A03::TSX(){}
void RP2A03::TXA(){}
void RP2A03::TXS(){}
void RP2A03::TYA(){}
void RP2A03::PHA(){}
void RP2A03::PHP(){}
void RP2A03::PLA(){}
void RP2A03::PLP(){}
void RP2A03::ASL(){}
void RP2A03::LSR(){}
void RP2A03::ROL(){}
void RP2A03::ROR(){}
void RP2A03::AND(){}
void RP2A03::BIT(){}
void RP2A03::EOR(){}
void RP2A03::ORA(){}
void RP2A03::ADC(){}
void RP2A03::CMP(){}
void RP2A03::CPX(){}
void RP2A03::CPY(){}
void RP2A03::SBC(){}
void RP2A03::DEC(){} 
void RP2A03::DEX(){} 
void RP2A03::DEY(){} 
void RP2A03::INC(){} 
void RP2A03::INX(){}
void RP2A03::INY(){}
void RP2A03::BRK(){}
void RP2A03::JMP(){}
void RP2A03::JSR(){}
void RP2A03::RTI(){}
void RP2A03::RTS(){}
void RP2A03::BCC(){}
void RP2A03::BCS(){}
void RP2A03::BEQ(){}
void RP2A03::BMI(){}
void RP2A03::BNE(){}
void RP2A03::BPL(){} 
void RP2A03::BVC(){}
void RP2A03::BVS(){}
void RP2A03::CLC(){}
void RP2A03::CLD(){}
void RP2A03::CLI(){}
void RP2A03::CLV(){}
void RP2A03::SEC(){}
void RP2A03::SED(){}
void RP2A03::SEI(){}
void RP2A03::NOP(){}

/************************************
 *          Addressing Modes        *
 ************************************/

void RP2A03::IMPLI(){}
void RP2A03::ACCUM(){}
void RP2A03::IMMED(){}
void RP2A03::ABSOL(){}
void RP2A03::XIABS(){}
void RP2A03::YIABS(){}
void RP2A03::ABSIN(){}
void RP2A03::ZPAGE(){}
void RP2A03::XIZPG(){}
void RP2A03::YIZPG(){}
void RP2A03::XIZPI(){}
void RP2A03::ZPIYI(){}
void RP2A03::RELAT(){}


/****************************************
 *  Instruction Vector Null Functions   *
 ****************************************/

void RP2A03::NII(){}
void RP2A03::NIVIM(){}