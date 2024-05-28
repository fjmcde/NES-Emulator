#ifndef RP2A03_H
#define RP2A03_H

/* Standard Headers */
#include <array>
#include <string>
/* Project Headers */
#include "bus.h"
#include "global.h"
#include "nesmemory.h"

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
        Reg16 PC;   /* 16-bit Programm Counter Register */
        Reg8 SP;    /* 8-bit Stack Pointer Register */
        Reg8 A;     /* 8-bit Accumulator Register */
        Reg8 X;     /* 8-bit Index Register */
        Reg8 Y;     /* 8-bit Index Register (can't be used with SP) */

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
        }status;

        /* Connected memory bus */
        Bus* memBus;

        /* Instruction Execution Functions */
        /* Load/Store */
        void LDA(U8 a, U8 b); void LDX(U8 a, U8 b); void LDY(U8 a, U8 b); void STA(U8 a, U8 b); void STX(U8 a, U8 b); void STY(U8 a, U8 b);
        /* Transfer */
        void TAX(U8 a, U8 b); void TAY(U8 a, U8 b); void TSX(U8 a, U8 b); void TXA(U8 a, U8 b); void TXS(U8 a, U8 b); void TYA(U8 a, U8 b);
        /* Stack */
        void PHA(U8 a, U8 b); void PHP(U8 a, U8 b); void PLA(U8 a, U8 b); void PLP(U8 a, U8 b);
        /* Shift */
        void ASL(U8 a, U8 b); void LSR(U8 a, U8 b); void ROL(U8 a, U8 b); void ROR(U8 a, U8 b);
        /* Logic */
        void AND(U8 a, U8 b); void BIT(U8 a, U8 b); void EOR(U8 a, U8 b); void ORA(U8 a, U8 b);
        /* Arithmetic */
        void ADC(U8 a, U8 b); void CMP(U8 a, U8 b); void CPX(U8 a, U8 b); void CPY(U8 a, U8 b); void SBC(U8 a, U8 b);
        /* Increment/Decrement */
        void DEC(U8 a, U8 b); void DEX(U8 a, U8 b); void DEY(U8 a, U8 b); void INC(U8 a, U8 b); void INX(U8 a, U8 b); void INY(U8 a, U8 b);
        /* Control Flow */
        void BRK(U8 a, U8 b); void JMP(U8 a, U8 b); void JSR(U8 a, U8 b); void RTI(U8 a, U8 b); void RTS(U8 a, U8 b);
        /* Branch */
        void BCC(U8 a, U8 b); void BCS(U8 a, U8 b); void BEQ(U8 a, U8 b); void BMI(U8 a, U8 b); void BNE(U8 a, U8 b); void BPL(U8 a, U8 b); void BVC(U8 a, U8 b); void BVS(U8 a, U8 b);
        /* Flags */
        void CLC(U8 a, U8 b); void CLD(U8 a, U8 b); void CLI(U8 a, U8 b); void CLV(U8 a, U8 b); void SEC(U8 a, U8 b); void SED(U8 a, U8 b); void SEI(U8 a, U8 b);
        /* No Operation */
        void NOP(U8 a, U8 b);
        /* NULL Index Instruction */
        void NII(U8 a, U8 b);

        /* Addressing Mode Functions */
        enum class AddrMode
        {
            impli,
            accum,
            immed,
            absol,
            xiabs,
            yiabs,
            absin,
            zpage,
            xizpg,
            yizpg,
            xizpi,
            yizpi,
            relat,
            nivim
        };

        /* Instruction Prototype */
        struct Instr_t
        {
            std::string mnumonic;                   /* Mnumonic OpCode */
            AddrMode addrMode;                      /* Addressing Mode */
            void (RP2A03::*func)(U8 a, U8 b);       /* Instruction function pointer */
            U8 opCode;                              /* Operation Code (hex) */
            U8 length;                              /* Length of instruction in bytes */
            U8 cycles;                              /* Number of machine cycles */
            U8* firstOperand;                       /* First byte obtained via addressing mode */
            U8* secondOperand;                      /* Second byte obtained via addressing mode */
        };

        /* Instruction Vector */
        const std::array<RP2A03::Instr_t, 256> instr = {{
            {"BRK", AddrMode::impli, &RP2A03::BRK, 0x00, 1, 7, nullptr, nullptr},
            {"ORA", AddrMode::xizpi, &RP2A03::ORA, 0x01, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x02, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x03, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x04, 0, 0, nullptr, nullptr},
            {"ORA", AddrMode::zpage, &RP2A03::ORA, 0x05, 2, 3, nullptr, nullptr},
            {"ASL", AddrMode::zpage, &RP2A03::ASL, 0x06, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x07, 0, 0, nullptr, nullptr},
            {"PHP", AddrMode::impli, &RP2A03::PHP, 0x08, 1, 3, nullptr, nullptr},
            {"ORA", AddrMode::immed, &RP2A03::ORA, 0x09, 2, 2, nullptr, nullptr},
            {"ASL", AddrMode::accum, &RP2A03::ASL, 0x0A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0C, 0, 0, nullptr, nullptr},
            {"ORA", AddrMode::absol, &RP2A03::ORA, 0x0D, 3, 4, nullptr, nullptr},
            {"ASL", AddrMode::absol, &RP2A03::ASL, 0x0E, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0F, 0, 0, nullptr, nullptr},
            {"BPL", AddrMode::relat, &RP2A03::BPL, 0x10, 2, 2, nullptr, nullptr},
            {"ORA", AddrMode::yizpi, &RP2A03::ORA, 0x11, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x12, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x13, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x14, 0, 0, nullptr, nullptr},
            {"ORA", AddrMode::xizpg, &RP2A03::ORA, 0x15, 2, 4, nullptr, nullptr},
            {"ASL", AddrMode::xizpg, &RP2A03::ASL, 0x16, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x17, 0, 0, nullptr, nullptr},
            {"CLC", AddrMode::impli, &RP2A03::CLC, 0x18, 1, 2, nullptr, nullptr},
            {"ORA", AddrMode::yiabs, &RP2A03::ORA, 0x19, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1A, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1C, 0, 0, nullptr, nullptr},
            {"ORA", AddrMode::xiabs, &RP2A03::ORA, 0x1D, 3, 4, nullptr, nullptr},
            {"ASL", AddrMode::xiabs, &RP2A03::ASL, 0x1E, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1F, 0, 0, nullptr, nullptr},
            {"JSR", AddrMode::absol, &RP2A03::JSR, 0x20, 3, 6, nullptr, nullptr},
            {"AND", AddrMode::xizpi, &RP2A03::AND, 0x21, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x22, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x23, 0, 0, nullptr, nullptr},
            {"BIT", AddrMode::zpage, &RP2A03::BIT, 0x24, 2, 3, nullptr, nullptr},
            {"AND", AddrMode::zpage, &RP2A03::AND, 0x25, 2, 3, nullptr, nullptr},
            {"ROL", AddrMode::zpage, &RP2A03::ROL, 0x26, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x27, 0, 0, nullptr, nullptr},
            {"PLP", AddrMode::impli, &RP2A03::PLP, 0x28, 1, 4, nullptr, nullptr},
            {"AND", AddrMode::immed, &RP2A03::AND, 0x29, 2, 2, nullptr, nullptr},
            {"ROL", AddrMode::accum, &RP2A03::ROL, 0x2A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x2B, 0, 0, nullptr, nullptr},
            {"BIT", AddrMode::absol, &RP2A03::BIT, 0x2C, 3, 4, nullptr, nullptr},
            {"AND", AddrMode::absol, &RP2A03::AND, 0x2D, 3, 4, nullptr, nullptr},
            {"ROL", AddrMode::absol, &RP2A03::ROL, 0x2E, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x2F, 0, 0, nullptr, nullptr},
            {"BMI", AddrMode::relat, &RP2A03::BMI, 0x30, 2, 2, nullptr, nullptr},
            {"AND", AddrMode::yizpi, &RP2A03::AND, 0x31, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x32, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x33, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x34, 0, 0, nullptr, nullptr},
            {"AND", AddrMode::xizpg, &RP2A03::AND, 0x35, 2, 4, nullptr, nullptr},
            {"ROL", AddrMode::xizpg, &RP2A03::ROL, 0x36, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x37, 0, 0, nullptr, nullptr},
            {"SEC", AddrMode::impli, &RP2A03::SEC, 0x38, 1, 2, nullptr, nullptr},
            {"AND", AddrMode::yiabs, &RP2A03::AND, 0x39, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3A, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3C, 0, 0, nullptr, nullptr},
            {"AND", AddrMode::xiabs, &RP2A03::AND, 0x3D, 3, 4, nullptr, nullptr},
            {"ROL", AddrMode::xiabs, &RP2A03::ROL, 0x3E, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3F, 0, 0, nullptr, nullptr},
            {"RTI", AddrMode::impli, &RP2A03::RTI, 0x40, 1, 6, nullptr, nullptr},
            {"EOR", AddrMode::xizpi, &RP2A03::EOR, 0x41, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x42, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x43, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x44, 0, 0, nullptr, nullptr},
            {"EOR", AddrMode::zpage, &RP2A03::EOR, 0x45, 2, 3, nullptr, nullptr},
            {"LSR", AddrMode::zpage, &RP2A03::LSR, 0x46, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x47, 0, 0, nullptr, nullptr},
            {"PHA", AddrMode::impli, &RP2A03::PHA, 0x48, 1, 3, nullptr, nullptr},
            {"EOR", AddrMode::immed, &RP2A03::EOR, 0x49, 2, 2, nullptr, nullptr},
            {"LSR", AddrMode::accum, &RP2A03::LSR, 0x4A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x4B, 0, 0, nullptr, nullptr},
            {"JMP", AddrMode::absol, &RP2A03::JMP, 0x4C, 3, 3, nullptr, nullptr},
            {"EOR", AddrMode::absol, &RP2A03::EOR, 0x4D, 3, 4, nullptr, nullptr},
            {"LSR", AddrMode::absol, &RP2A03::LSR, 0x4E, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x4F, 0, 0, nullptr, nullptr},
            {"BVC", AddrMode::relat, &RP2A03::BVC, 0x50, 2, 2, nullptr, nullptr},
            {"EOR", AddrMode::yizpi, &RP2A03::EOR, 0x51, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x52, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x53, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x54, 0, 0, nullptr, nullptr},
            {"EOR", AddrMode::xizpg, &RP2A03::EOR, 0x55, 2, 4, nullptr, nullptr},
            {"LSR", AddrMode::xizpg, &RP2A03::LSR, 0x56, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x57, 0, 0, nullptr, nullptr},
            {"CLI", AddrMode::impli, &RP2A03::CLI, 0x58, 1, 2, nullptr, nullptr},
            {"EOR", AddrMode::yiabs, &RP2A03::EOR, 0x59, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5A, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5C, 0, 0, nullptr, nullptr},
            {"EOR", AddrMode::xiabs, &RP2A03::EOR, 0x5D, 3, 4, nullptr, nullptr},
            {"LSR", AddrMode::xiabs, &RP2A03::LSR, 0x5E, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5F, 0, 0, nullptr, nullptr},
            {"RTS", AddrMode::impli, &RP2A03::RTS, 0x60, 1, 6, nullptr, nullptr},
            {"ADC", AddrMode::xizpi, &RP2A03::ADC, 0x61, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x62, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x63, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x64, 0, 0, nullptr, nullptr},
            {"ADC", AddrMode::zpage, &RP2A03::ADC, 0x65, 2, 3, nullptr, nullptr},
            {"ROR", AddrMode::zpage, &RP2A03::ROR, 0x66, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x67, 0, 0, nullptr, nullptr},
            {"PLA", AddrMode::impli, &RP2A03::PLA, 0x68, 1, 4, nullptr, nullptr},
            {"ADC", AddrMode::immed, &RP2A03::ADC, 0x69, 2, 2, nullptr, nullptr},
            {"ROR", AddrMode::accum, &RP2A03::ROR, 0x6A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x6B, 0, 0, nullptr, nullptr},
            {"JMP", AddrMode::absin, &RP2A03::JMP, 0x6C, 3, 5, nullptr, nullptr},
            {"ADC", AddrMode::absol, &RP2A03::ADC, 0x6D, 3, 4, nullptr, nullptr},
            {"ROR", AddrMode::absol, &RP2A03::ROR, 0x6E, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x6F, 0, 0, nullptr, nullptr},
            {"BVS", AddrMode::relat, &RP2A03::BVS, 0x70, 2, 2, nullptr, nullptr},
            {"ADC", AddrMode::yizpi, &RP2A03::ADC, 0x71, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x72, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x73, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x74, 0, 0, nullptr, nullptr},
            {"ADC", AddrMode::xizpg, &RP2A03::ADC, 0x75, 2, 4, nullptr, nullptr},
            {"ROR", AddrMode::xizpg, &RP2A03::ROR, 0x76, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x77, 0, 0, nullptr, nullptr},
            {"SEI", AddrMode::impli, &RP2A03::SEI, 0x78, 1, 2, nullptr, nullptr},
            {"ADC", AddrMode::yiabs, &RP2A03::ADC, 0x79, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7A, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7C, 0, 0, nullptr, nullptr},
            {"ADC", AddrMode::xiabs, &RP2A03::ADC, 0x7D, 3, 4, nullptr, nullptr},
            {"ROR", AddrMode::xiabs, &RP2A03::ROR, 0x7E, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7F, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x80, 0, 0, nullptr, nullptr},
            {"STA", AddrMode::xizpi, &RP2A03::STA, 0x81, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x82, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x83, 0, 0, nullptr, nullptr},
            {"STY", AddrMode::zpage, &RP2A03::STY, 0x84, 2, 3, nullptr, nullptr},
            {"STA", AddrMode::zpage, &RP2A03::STA, 0x85, 2, 3, nullptr, nullptr},
            {"STX", AddrMode::zpage, &RP2A03::STX, 0x86, 2, 3, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x87, 0, 0, nullptr, nullptr},
            {"DEY", AddrMode::impli, &RP2A03::DEY, 0x88, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x89, 0, 0, nullptr, nullptr},
            {"TXA", AddrMode::impli, &RP2A03::TXA, 0x8A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x8B, 0, 0, nullptr, nullptr},
            {"STY", AddrMode::absol, &RP2A03::STY, 0x8C, 3, 4, nullptr, nullptr},
            {"STA", AddrMode::absol, &RP2A03::STA, 0x8D, 3, 4, nullptr, nullptr},
            {"STX", AddrMode::absol, &RP2A03::STX, 0x8E, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x8F, 0, 0, nullptr, nullptr},
            {"BCC", AddrMode::relat, &RP2A03::BCC, 0x90, 2, 2, nullptr, nullptr},
            {"STA", AddrMode::yizpi, &RP2A03::STA, 0x91, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x92, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x93, 0, 0, nullptr, nullptr},
            {"STY", AddrMode::xizpg, &RP2A03::STY, 0x94, 2, 4, nullptr, nullptr},
            {"STA", AddrMode::xizpg, &RP2A03::STA, 0x95, 2, 4, nullptr, nullptr},
            {"STX", AddrMode::yizpg, &RP2A03::STX, 0x96, 2, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x97, 0, 0, nullptr, nullptr},
            {"TYA", AddrMode::impli, &RP2A03::TYA, 0x98, 1, 2, nullptr, nullptr},
            {"STA", AddrMode::yiabs, &RP2A03::STA, 0x99, 3, 5, nullptr, nullptr},
            {"TSX", AddrMode::impli, &RP2A03::TSX, 0x9A, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9B, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9C, 0, 0, nullptr, nullptr},
            {"STA", AddrMode::xiabs, &RP2A03::STA, 0x9D, 3, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9E, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9F, 0, 0, nullptr, nullptr},
            {"LDY", AddrMode::immed, &RP2A03::LDY, 0xA0, 2, 2, nullptr, nullptr},
            {"LDA", AddrMode::xizpi, &RP2A03::LDA, 0xA1, 2, 6, nullptr, nullptr},
            {"LDX", AddrMode::immed, &RP2A03::LDX, 0xA2, 2, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xA3, 0, 0, nullptr, nullptr},
            {"LDY", AddrMode::zpage, &RP2A03::LDY, 0xA4, 2, 3, nullptr, nullptr},
            {"LDA", AddrMode::zpage, &RP2A03::LDA, 0xA5, 2, 3, nullptr, nullptr},
            {"LDX", AddrMode::zpage, &RP2A03::LDX, 0xA6, 2, 3, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xA7, 0, 0, nullptr, nullptr},
            {"TAY", AddrMode::impli, &RP2A03::TAY, 0xA8, 1, 2, nullptr, nullptr},
            {"LDA", AddrMode::immed, &RP2A03::LDA, 0xA9, 2, 2, nullptr, nullptr},
            {"TAX", AddrMode::impli, &RP2A03::TAX, 0xAA, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xAB, 0, 0, nullptr, nullptr},
            {"LDY", AddrMode::absol, &RP2A03::LDY, 0xAC, 3, 4, nullptr, nullptr},
            {"LDA", AddrMode::absol, &RP2A03::LDA, 0xAD, 3, 4, nullptr, nullptr},
            {"LDX", AddrMode::absol, &RP2A03::LDX, 0xAE, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xAF, 0, 0, nullptr, nullptr},
            {"BCS", AddrMode::relat, &RP2A03::BCS, 0xB0, 2, 2, nullptr, nullptr},
            {"LDA", AddrMode::yizpi, &RP2A03::LDA, 0xB1, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB2, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB3, 0, 0, nullptr, nullptr},
            {"LDY", AddrMode::xizpg, &RP2A03::LDY, 0xB4, 2, 4, nullptr, nullptr},
            {"LDA", AddrMode::xizpg, &RP2A03::LDA, 0xB5, 2, 4, nullptr, nullptr},
            {"LDX", AddrMode::yizpg, &RP2A03::LDX, 0xB6, 2, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB7, 0, 0, nullptr, nullptr},
            {"CLV", AddrMode::impli, &RP2A03::CLV, 0xB8, 1, 2, nullptr, nullptr},
            {"LDA", AddrMode::yiabs, &RP2A03::LDA, 0xB9, 3, 4, nullptr, nullptr},
            {"TSX", AddrMode::impli, &RP2A03::TSX, 0xBA, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xBB, 0, 0, nullptr, nullptr},
            {"LDY", AddrMode::xiabs, &RP2A03::LDY, 0xBC, 3, 4, nullptr, nullptr},
            {"LDA", AddrMode::xiabs, &RP2A03::LDA, 0xBD, 3, 4, nullptr, nullptr},
            {"LDX", AddrMode::yiabs, &RP2A03::LDX, 0xBE, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xBF, 0, 0, nullptr, nullptr},
            {"CPY", AddrMode::immed, &RP2A03::CPY, 0xC0, 2, 2, nullptr, nullptr},
            {"CMP", AddrMode::xizpi, &RP2A03::CMP, 0xC1, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC2, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC3, 0, 0, nullptr, nullptr},
            {"CPY", AddrMode::zpage, &RP2A03::CPY, 0xC4, 2, 3, nullptr, nullptr},
            {"CMP", AddrMode::zpage, &RP2A03::CMP, 0xC5, 2, 3, nullptr, nullptr},
            {"DEC", AddrMode::zpage, &RP2A03::DEC, 0xC6, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC7, 0, 0, nullptr, nullptr},
            {"INY", AddrMode::impli, &RP2A03::INY, 0xC8, 1, 2, nullptr, nullptr},
            {"CMP", AddrMode::immed, &RP2A03::CMP, 0xC9, 2, 2, nullptr, nullptr},
            {"DEX", AddrMode::impli, &RP2A03::DEX, 0xCA, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xCB, 0, 0, nullptr, nullptr},
            {"CPY", AddrMode::absol, &RP2A03::CPY, 0xCC, 3, 4, nullptr, nullptr},
            {"CMP", AddrMode::absol, &RP2A03::CMP, 0xCD, 3, 4, nullptr, nullptr},
            {"DEC", AddrMode::immed, &RP2A03::DEC, 0xCE, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xCF, 0, 0, nullptr, nullptr},
            {"BNE", AddrMode::relat, &RP2A03::BNE, 0xD0, 2, 2, nullptr, nullptr},
            {"CMP", AddrMode::yizpi, &RP2A03::CMP, 0xD1, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD2, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD3, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD4, 0, 0, nullptr, nullptr},
            {"CMP", AddrMode::xizpg, &RP2A03::CMP, 0xD5, 2, 4, nullptr, nullptr},
            {"DEC", AddrMode::xizpg, &RP2A03::DEC, 0xD6, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD7, 0, 0, nullptr, nullptr},
            {"CLD", AddrMode::impli, &RP2A03::CLD, 0xD8, 1, 2, nullptr, nullptr},
            {"CMP", AddrMode::yiabs, &RP2A03::CMP, 0xD9, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDA, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDB, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDC, 0, 0, nullptr, nullptr},
            {"CMP", AddrMode::xiabs, &RP2A03::CMP, 0xDD, 3, 4, nullptr, nullptr},
            {"DEC", AddrMode::xiabs, &RP2A03::DEC, 0xDE, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDF, 0, 0, nullptr, nullptr},
            {"CPX", AddrMode::immed, &RP2A03::CPX, 0xE0, 2, 2, nullptr, nullptr},
            {"SBC", AddrMode::xizpi, &RP2A03::SBC, 0xE1, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE2, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE3, 0, 0, nullptr, nullptr},
            {"CPX", AddrMode::zpage, &RP2A03::CPX, 0xE4, 2, 3, nullptr, nullptr},
            {"SBC", AddrMode::zpage, &RP2A03::SBC, 0xE5, 2, 3, nullptr, nullptr},
            {"INC", AddrMode::zpage, &RP2A03::INC, 0xE6, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE7, 0, 0, nullptr, nullptr},
            {"INX", AddrMode::impli, &RP2A03::INX, 0xE8, 1, 2, nullptr, nullptr},
            {"SBC", AddrMode::immed, &RP2A03::SBC, 0xE9, 2, 2, nullptr, nullptr},
            {"NOP", AddrMode::impli, &RP2A03::NOP, 0xEA, 1, 2, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xEB, 0, 0, nullptr, nullptr},
            {"CPX", AddrMode::absol, &RP2A03::CPX, 0xEC, 3, 4, nullptr, nullptr},
            {"SBC", AddrMode::absol, &RP2A03::SBC, 0xED, 3, 4, nullptr, nullptr},
            {"INC", AddrMode::absol, &RP2A03::INC, 0xEE, 3, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xEF, 0, 0, nullptr, nullptr},
            {"BEQ", AddrMode::relat, &RP2A03::BEQ, 0xF0, 2, 2, nullptr, nullptr},
            {"SBC", AddrMode::yizpi, &RP2A03::SBC, 0xF1, 2, 5, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF2, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF3, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF4, 0, 0, nullptr, nullptr},
            {"SBC", AddrMode::xizpg, &RP2A03::SBC, 0xF5, 2, 4, nullptr, nullptr},
            {"INC", AddrMode::xizpg, &RP2A03::INC, 0xF6, 2, 6, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF7, 0, 0, nullptr, nullptr},
            {"SED", AddrMode::impli, &RP2A03::SED, 0xF8, 1, 2, nullptr, nullptr},
            {"SBC", AddrMode::yiabs, &RP2A03::SBC, 0xF9, 3, 4, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFA, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFB, 0, 0, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFC, 0, 0, nullptr, nullptr},
            {"SBC", AddrMode::xiabs, &RP2A03::SBC, 0xFD, 3, 4, nullptr, nullptr},
            {"INC", AddrMode::xiabs, &RP2A03::INC, 0xFE, 3, 7, nullptr, nullptr},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFF, 0, 0, nullptr, nullptr},
        }};

        /* Psuedo-pipeline member objects */
        Instr_t currentInstruction;     /* Currently instruction being executed */
        U16 returnValue;                /* Instruction return value */

        /* Pseudo-pipeline member functions */
        U8 fetch(void);
        void decode(U8 opCode);
        void applyAddressingMode(void);
        void executeInstruction(void);

    public:
        RP2A03();
        ~RP2A03();

        /* Public Member functions */
        void CPU_Cycle(void);

        /* Interrupt Handlers */
        void reset(void);
        void NMI(void);
        void IRQ(void);

        /* Assessors */
        inline Reg16 getPC(void) { return PC; }
        inline Reg8 getSP(void) { return SP; }
        inline Reg8 getA(void) { return A; }
        inline Reg8 getX(void) { return X; }
        inline Reg8 getY(void) { return Y; }
        inline Flag8 getStatus(void) { return status; }

        /* Modifiers */
        void setFlag(U8 flags);
};


#endif /* RP2A03_H */