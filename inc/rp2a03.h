#ifndef RP2A03_H
#define RP2A03_H

/* Standard Headers */
#include <array>
#include <string>
/* Project Headers */
#include "global.h"

/* Constant Expressions: CPU Status Flags */
constexpr U8 carryFlag               = 0x01;
constexpr U8 zeroFlag                = 0x02;
constexpr U8 interruptDisableFlag    = 0x04;
constexpr U8 decimalModeFlag         = 0x08;
constexpr U8 breakFlag               = 0x10;
constexpr U8 unusedFlag              = 0x20;
constexpr U8 overflowFlag            = 0x40;
constexpr U8 negativeFlag            = 0x80;


class RP2A03
{
    private:
        /* Registers */
        Reg16* PC;   /* 16-bit Programm Counter Register */
        Reg8* SP;    /* 8-bit Stack Pointer Register */
        Reg8* A;     /* 8-bit Accumulator Register */
        Reg8* X;     /* 8-bit Index Register */
        Reg8* Y;     /* 8-bit Index Register (can't be used with SP) */

        /* 8-bit CPU Status Register */
        struct Flag8
        {
            U8 C : 1;       /* BIT0: Carry Flag */
            U8 Z : 1;       /* BIT1: Zero Flag */
            U8 I : 1;       /* BIT2: Interrupt Disabled Flag */
            U8 D : 1;       /* BIT3: Decimal Mode Flag (Holdover from 6502; NOT USED) */
            U8 B : 1;       /* BIT4: Break Flag */
            U8 UNUSED : 1;  /* BIT5: UNUSED */
            U8 V : 1;       /* BIT6: Overflow Flag */
            U8 N : 1;       /* BIT7: Negative Flag */
        }*status;

        

        /* Instruction Execution Functions */
        void LDA(); void LDX(); void LDY(); void STA(); void STX(); void STY();                             /* Load/Store */
        void TAX(); void TAY(); void TSX(); void TXA(); void TXS(); void TYA();                             /* Transfer */
        void PHA(); void PHP(); void PLA(); void PLP();                                                     /* Stack */
        void ASL(); void LSR(); void ROL(); void ROR();                                                     /* Shift */
        void AND(); void BIT(); void EOR(); void ORA();                                                     /* Logic */
        void ADC(); void CMP(); void CPX(); void CPY(); void SBC();                                         /* Arithmetic */
        void DEC(); void DEX(); void DEY(); void INC(); void INX(); void INY();                             /* Increment/Decrement */
        void BRK(); void JMP(); void JSR(); void RTI(); void RTS();                                         /* Control Flow */
        void BCC(); void BCS(); void BEQ(); void BMI(); void BNE(); void BPL(); void BVC(); void BVS();     /* Branch */
        void CLC(); void CLD(); void CLI(); void CLV(); void SEC(); void SED(); void SEI();                 /* Flags */
        void NOP();                                                                                         /* No Operation */

        /* Addressing Mode Functions */
        void IMPLI(); void ACCUM(); void IMMED(); void ABSOL(); void XIABS(); void YIABS(); 
        void ABSIN(); void ZPAGE(); void XIZPG(); void YIZPG(); void XIZPI(); void ZPIYI();
        void RELAT();

        /* Instruction Vector Null Functions */
        void NII();     /* Null Instruction Index (Instruction) */
        void NIVIM();   /* Null Instruction Vector Index Mode (Address Mode) */

        /* Instruction Prototype */
        struct Instr_t
        {
            std::string mnumonic;           /* Mnumonic OpCode */
            void (RP2A03::*addrMode)(void); /* Addressing Mode function pointer  */
            void (RP2A03::*execute)(void);  /* Instruction Execute function pointer */
            U8 opCode;                      /* Operation Code (hex) */
            U8 length;                      /* Length of instruction in bytes */
            U8 cycles;                      /* Number of machine cycles */
        };

        /* Instruction Vector */
        const std::array<RP2A03::Instr_t, 256> instr = {{
            {"BRK", &RP2A03::IMPLI, &RP2A03::BRK, 0x00, 1, 7},
            {"ORA", &RP2A03::XIZPI, &RP2A03::ORA, 0x01, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x02, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x03, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x04, 0, 0},
            {"ORA", &RP2A03::ZPAGE, &RP2A03::ORA, 0x05, 2, 3},
            {"ASL", &RP2A03::ZPAGE, &RP2A03::ASL, 0x06, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x07, 0, 0},
            {"PHP", &RP2A03::IMPLI, &RP2A03::PHP, 0x08, 1, 3},
            {"ORA", &RP2A03::IMMED, &RP2A03::ORA, 0x09, 2, 2},
            {"ASL", &RP2A03::ACCUM, &RP2A03::ASL, 0x0A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x0B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x0C, 0, 0},
            {"ORA", &RP2A03::ABSOL, &RP2A03::ORA, 0x0D, 3, 4},
            {"ASL", &RP2A03::ABSOL, &RP2A03::ASL, 0x0E, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x0F, 0, 0},
            {"BPL", &RP2A03::RELAT, &RP2A03::BPL, 0x10, 2, 2},
            {"ORA", &RP2A03::ZPIYI, &RP2A03::ORA, 0x11, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x12, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x13, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x14, 0, 0},
            {"ORA", &RP2A03::XIZPG, &RP2A03::ORA, 0x15, 2, 4},
            {"ASL", &RP2A03::XIZPG, &RP2A03::ASL, 0x16, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x17, 0, 0},
            {"CLC", &RP2A03::IMPLI, &RP2A03::CLC, 0x18, 1, 2},
            {"ORA", &RP2A03::YIABS, &RP2A03::ORA, 0x19, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x1A, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x1B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x1C, 0, 0},
            {"ORA", &RP2A03::XIABS, &RP2A03::ORA, 0x1D, 3, 4},
            {"ASL", &RP2A03::XIABS, &RP2A03::ASL, 0x1E, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x1F, 0, 0},
            {"JSR", &RP2A03::ABSOL, &RP2A03::JSR, 0x20, 3, 6},
            {"AND", &RP2A03::XIZPI, &RP2A03::AND, 0x21, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x22, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x23, 0, 0},
            {"BIT", &RP2A03::ZPAGE, &RP2A03::BIT, 0x24, 2, 3},
            {"AND", &RP2A03::ZPAGE, &RP2A03::AND, 0x25, 2, 3},
            {"ROL", &RP2A03::ZPAGE, &RP2A03::ROL, 0x26, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x27, 0, 0},
            {"PLP", &RP2A03::IMPLI, &RP2A03::PLP, 0x28, 1, 4},
            {"AND", &RP2A03::IMMED, &RP2A03::AND, 0x29, 2, 2},
            {"ROL", &RP2A03::ACCUM, &RP2A03::ROL, 0x2A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x2B, 0, 0},
            {"BIT", &RP2A03::ABSOL, &RP2A03::BIT, 0x2C, 3, 4},
            {"AND", &RP2A03::ABSOL, &RP2A03::AND, 0x2D, 3, 4},
            {"ROL", &RP2A03::ABSOL, &RP2A03::ROL, 0x2E, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x2F, 0, 0},
            {"BMI", &RP2A03::RELAT, &RP2A03::BMI, 0x30, 2, 2},
            {"AND", &RP2A03::ZPIYI, &RP2A03::AND, 0x31, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x32, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x33, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x34, 0, 0},
            {"AND", &RP2A03::XIZPG, &RP2A03::AND, 0x35, 2, 4},
            {"ROL", &RP2A03::XIZPG, &RP2A03::ROL, 0x36, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x37, 0, 0},
            {"SEC", &RP2A03::IMPLI, &RP2A03::SEC, 0x38, 1, 2},
            {"AND", &RP2A03::YIABS, &RP2A03::AND, 0x39, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x3A, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x3B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x3C, 0, 0},
            {"AND", &RP2A03::XIABS, &RP2A03::AND, 0x3D, 3, 4},
            {"ROL", &RP2A03::XIABS, &RP2A03::ROL, 0x3E, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x3F, 0, 0},
            {"RTI", &RP2A03::IMPLI, &RP2A03::RTI, 0x40, 1, 6},
            {"EOR", &RP2A03::XIZPI, &RP2A03::EOR, 0x41, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x42, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x43, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x44, 0, 0},
            {"EOR", &RP2A03::ZPAGE, &RP2A03::EOR, 0x45, 2, 3},
            {"LSR", &RP2A03::ZPAGE, &RP2A03::LSR, 0x46, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x47, 0, 0},
            {"PHA", &RP2A03::IMPLI, &RP2A03::PHA, 0x48, 1, 3},
            {"EOR", &RP2A03::IMMED, &RP2A03::EOR, 0x49, 2, 2},
            {"LSR", &RP2A03::ACCUM, &RP2A03::LSR, 0x4A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x4B, 0, 0},
            {"JMP", &RP2A03::ABSOL, &RP2A03::JMP, 0x4C, 3, 3},
            {"EOR", &RP2A03::ABSOL, &RP2A03::EOR, 0x4D, 3, 4},
            {"LSR", &RP2A03::ABSOL, &RP2A03::LSR, 0x4E, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x4F, 0, 0},
            {"BVC", &RP2A03::RELAT, &RP2A03::BVC, 0x50, 2, 2},
            {"EOR", &RP2A03::ZPIYI, &RP2A03::EOR, 0x51, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x52, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x53, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x54, 0, 0},
            {"EOR", &RP2A03::XIZPG, &RP2A03::EOR, 0x55, 2, 4},
            {"LSR", &RP2A03::XIZPG, &RP2A03::LSR, 0x56, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x57, 0, 0},
            {"CLI", &RP2A03::IMPLI, &RP2A03::CLI, 0x58, 1, 2},
            {"EOR", &RP2A03::YIABS, &RP2A03::EOR, 0x59, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x5A, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x5B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x5C, 0, 0},
            {"EOR", &RP2A03::XIABS, &RP2A03::EOR, 0x5D, 3, 4},
            {"LSR", &RP2A03::XIABS, &RP2A03::LSR, 0x5E, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x5F, 0, 0},
            {"RTS", &RP2A03::IMPLI, &RP2A03::RTS, 0x60, 1, 6},
            {"ADC", &RP2A03::XIZPI, &RP2A03::ADC, 0x61, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x62, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x63, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x64, 0, 0},
            {"ADC", &RP2A03::ZPAGE, &RP2A03::ADC, 0x65, 2, 3},
            {"ROR", &RP2A03::ZPAGE, &RP2A03::ROR, 0x66, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x67, 0, 0},
            {"PLA", &RP2A03::IMPLI, &RP2A03::PLA, 0x68, 1, 4},
            {"ADC", &RP2A03::IMMED, &RP2A03::ADC, 0x69, 2, 2},
            {"ROR", &RP2A03::ACCUM, &RP2A03::ROR, 0x6A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x6B, 0, 0},
            {"JMP", &RP2A03::ABSIN, &RP2A03::JMP, 0x6C, 3, 5},
            {"ADC", &RP2A03::ABSOL, &RP2A03::ADC, 0x6D, 3, 4},
            {"ROR", &RP2A03::ABSOL, &RP2A03::ROR, 0x6E, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x6F, 0, 0},
            {"BVS", &RP2A03::RELAT, &RP2A03::BVS, 0x70, 2, 2},
            {"ADC", &RP2A03::ZPIYI, &RP2A03::ADC, 0x71, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x72, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x73, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x74, 0, 0},
            {"ADC", &RP2A03::XIZPG, &RP2A03::ADC, 0x75, 2, 4},
            {"ROR", &RP2A03::XIZPG, &RP2A03::ROR, 0x76, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x77, 0, 0},
            {"SEI", &RP2A03::IMPLI, &RP2A03::SEI, 0x78, 1, 2},
            {"ADC", &RP2A03::YIABS, &RP2A03::ADC, 0x79, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x7A, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x7B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x7C, 0, 0},
            {"ADC", &RP2A03::XIABS, &RP2A03::ADC, 0x7D, 3, 4},
            {"ROR", &RP2A03::XIABS, &RP2A03::ROR, 0x7E, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x7F, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x80, 0, 0},
            {"STA", &RP2A03::XIZPI, &RP2A03::STA, 0x81, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x82, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x83, 0, 0},
            {"STY", &RP2A03::ZPAGE, &RP2A03::STY, 0x84, 2, 3},
            {"STA", &RP2A03::ZPAGE, &RP2A03::STA, 0x85, 2, 3},
            {"STX", &RP2A03::ZPAGE, &RP2A03::STX, 0x86, 2, 3},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x87, 0, 0},
            {"DEY", &RP2A03::IMPLI, &RP2A03::DEY, 0x88, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x89, 0, 0},
            {"TXA", &RP2A03::IMPLI, &RP2A03::TXA, 0x8A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x8B, 0, 0},
            {"STY", &RP2A03::ABSOL, &RP2A03::STY, 0x8C, 3, 4},
            {"STA", &RP2A03::ABSOL, &RP2A03::STA, 0x8D, 3, 4},
            {"STX", &RP2A03::ABSOL, &RP2A03::STX, 0x8E, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x8F, 0, 0},
            {"BCC", &RP2A03::RELAT, &RP2A03::BCC, 0x90, 2, 2},
            {"STA", &RP2A03::ZPIYI, &RP2A03::STA, 0x91, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x92, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x93, 0, 0},
            {"STY", &RP2A03::XIZPG, &RP2A03::STY, 0x94, 2, 4},
            {"STA", &RP2A03::XIZPG, &RP2A03::STA, 0x95, 2, 4},
            {"STX", &RP2A03::YIZPG, &RP2A03::STX, 0x96, 2, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x97, 0, 0},
            {"TYA", &RP2A03::IMPLI, &RP2A03::TYA, 0x98, 1, 2},
            {"STA", &RP2A03::YIABS, &RP2A03::STA, 0x99, 3, 5},
            {"TSX", &RP2A03::IMPLI, &RP2A03::TSX, 0x9A, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x9B, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x9C, 0, 0},
            {"STA", &RP2A03::XIABS, &RP2A03::STA, 0x9D, 3, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x9E, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0x9F, 0, 0},
            {"LDY", &RP2A03::IMMED, &RP2A03::LDY, 0xA0, 2, 2},
            {"LDA", &RP2A03::XIZPI, &RP2A03::LDA, 0xA1, 2, 6},
            {"LDX", &RP2A03::IMMED, &RP2A03::LDX, 0xA2, 2, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xA3, 0, 0},
            {"LDY", &RP2A03::ZPAGE, &RP2A03::LDY, 0xA4, 2, 3},
            {"LDA", &RP2A03::ZPAGE, &RP2A03::LDA, 0xA5, 2, 3},
            {"LDX", &RP2A03::ZPAGE, &RP2A03::LDX, 0xA6, 2, 3},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xA7, 0, 0},
            {"TAY", &RP2A03::IMPLI, &RP2A03::TAY, 0xA8, 1, 2},
            {"LDA", &RP2A03::IMMED, &RP2A03::LDA, 0xA9, 2, 2},
            {"TAX", &RP2A03::IMPLI, &RP2A03::TAX, 0xAA, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xAB, 0, 0},
            {"LDY", &RP2A03::ABSOL, &RP2A03::LDY, 0xAC, 3, 4},
            {"LDA", &RP2A03::ABSOL, &RP2A03::LDA, 0xAD, 3, 4},
            {"LDX", &RP2A03::ABSOL, &RP2A03::LDX, 0xAE, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xAF, 0, 0},
            {"BCS", &RP2A03::RELAT, &RP2A03::BCS, 0xB0, 2, 2},
            {"LDA", &RP2A03::ZPIYI, &RP2A03::LDA, 0xB1, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xB2, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xB3, 0, 0},
            {"LDY", &RP2A03::XIZPG, &RP2A03::LDY, 0xB4, 2, 4},
            {"LDA", &RP2A03::XIZPG, &RP2A03::LDA, 0xB5, 2, 4},
            {"LDX", &RP2A03::YIZPG, &RP2A03::LDX, 0xB6, 2, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xB7, 0, 0},
            {"CLV", &RP2A03::IMPLI, &RP2A03::CLV, 0xB8, 1, 2},
            {"LDA", &RP2A03::YIABS, &RP2A03::LDA, 0xB9, 3, 4},
            {"TSX", &RP2A03::IMPLI, &RP2A03::TSX, 0xBA, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xBB, 0, 0},
            {"LDY", &RP2A03::XIABS, &RP2A03::LDY, 0xBC, 3, 4},
            {"LDA", &RP2A03::XIABS, &RP2A03::LDA, 0xBD, 3, 4},
            {"LDX", &RP2A03::YIABS, &RP2A03::LDX, 0xBE, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xBF, 0, 0},
            {"CPY", &RP2A03::IMMED, &RP2A03::CPY, 0xC0, 2, 2},
            {"CMP", &RP2A03::XIZPI, &RP2A03::CMP, 0xC1, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xC2, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xC3, 0, 0},
            {"CPY", &RP2A03::ZPAGE, &RP2A03::CPY, 0xC4, 2, 3},
            {"CMP", &RP2A03::ZPAGE, &RP2A03::CMP, 0xC5, 2, 3},
            {"DEC", &RP2A03::ZPAGE, &RP2A03::DEC, 0xC6, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xC7, 0, 0},
            {"INY", &RP2A03::IMPLI, &RP2A03::INY, 0xC8, 1, 2},
            {"CMP", &RP2A03::IMMED, &RP2A03::CMP, 0xC9, 2, 2},
            {"DEX", &RP2A03::IMPLI, &RP2A03::DEX, 0xCA, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xCB, 0, 0},
            {"CPY", &RP2A03::ABSOL, &RP2A03::CPY, 0xCC, 3, 4},
            {"CMP", &RP2A03::ABSOL, &RP2A03::CMP, 0xCD, 3, 4},
            {"DEC", &RP2A03::IMMED, &RP2A03::DEC, 0xCE, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xCF, 0, 0},
            {"BNE", &RP2A03::RELAT, &RP2A03::BNE, 0xD0, 2, 2},
            {"CMP", &RP2A03::ZPIYI, &RP2A03::CMP, 0xD1, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xD2, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xD3, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xD4, 0, 0},
            {"CMP", &RP2A03::XIZPG, &RP2A03::CMP, 0xD5, 2, 4},
            {"DEC", &RP2A03::XIZPG, &RP2A03::DEC, 0xD6, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xD7, 0, 0},
            {"CLD", &RP2A03::IMPLI, &RP2A03::CLD, 0xD8, 1, 2},
            {"CMP", &RP2A03::YIABS, &RP2A03::CMP, 0xD9, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xDA, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xDB, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xDC, 0, 0},
            {"CMP", &RP2A03::XIABS, &RP2A03::CMP, 0xDD, 3, 4},
            {"DEC", &RP2A03::XIABS, &RP2A03::DEC, 0xDE, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xDF, 0, 0},
            {"CPX", &RP2A03::IMMED, &RP2A03::CPX, 0xE0, 2, 2},
            {"SBC", &RP2A03::XIZPI, &RP2A03::SBC, 0xE1, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xE2, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xE3, 0, 0},
            {"CPX", &RP2A03::ZPAGE, &RP2A03::CPX, 0xE4, 2, 3},
            {"SBC", &RP2A03::ZPAGE, &RP2A03::SBC, 0xE5, 2, 3},
            {"INC", &RP2A03::ZPAGE, &RP2A03::INC, 0xE6, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xE7, 0, 0},
            {"INX", &RP2A03::IMPLI, &RP2A03::INX, 0xE8, 1, 2},
            {"SBC", &RP2A03::IMMED, &RP2A03::SBC, 0xE9, 2, 2},
            {"NOP", &RP2A03::IMPLI, &RP2A03::NOP, 0xEA, 1, 2},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xEB, 0, 0},
            {"CPX", &RP2A03::ABSOL, &RP2A03::CPX, 0xEC, 3, 4},
            {"SBC", &RP2A03::ABSOL, &RP2A03::SBC, 0xED, 3, 4},
            {"INC", &RP2A03::ABSOL, &RP2A03::INC, 0xEE, 3, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xEF, 0, 0},
            {"BEQ", &RP2A03::RELAT, &RP2A03::BEQ, 0xF0, 2, 2},
            {"SBC", &RP2A03::ZPIYI, &RP2A03::SBC, 0xF1, 2, 5},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xF2, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xF3, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xF4, 0, 0},
            {"SBC", &RP2A03::XIZPG, &RP2A03::SBC, 0xF5, 2, 4},
            {"INC", &RP2A03::XIZPG, &RP2A03::INC, 0xF6, 2, 6},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xF7, 0, 0},
            {"SED", &RP2A03::IMPLI, &RP2A03::SED, 0xF8, 1, 2},
            {"SBC", &RP2A03::YIABS, &RP2A03::SBC, 0xF9, 3, 4},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xFA, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xFB, 0, 0},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xFC, 0, 0},
            {"SBC", &RP2A03::XIABS, &RP2A03::SBC, 0xFD, 3, 4},
            {"INC", &RP2A03::XIABS, &RP2A03::INC, 0xFE, 3, 7},
            {"NII", &RP2A03::NIVIM, &RP2A03::NII, 0xFF, 0, 0},
        }};


    public:
        RP2A03();
        ~RP2A03();

        /* Pseudo-pipeline */
        void fetch(void);
        void decode(U8 byteCode);

        /* Interrupt Handlers */
        void reset(void);
        void NMI(void);
        void IRQ(void);

        /* Assessors */
        inline Reg16 getPC(void) { return *PC; }
        inline Reg8 getSP(void) { return *SP; }
        inline Reg8 getA(void) { return *A; }
        inline Reg8 getX(void) { return *X; }
        inline Reg8 getY(void) { return *Y; }
        inline Flag8 getStatus(void) { return *status; }

        /* Modifiers */
        void setFlag(U8 flags);
};


#endif /* RP2A03_H */