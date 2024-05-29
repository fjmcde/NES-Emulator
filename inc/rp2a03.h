#ifndef RP2A03_H
#define RP2A03_H

/* Standard Headers */
#include <array>
#include <string>
/* Project Headers */
#include "bus.h"
#include "global.h"
#include "nesmemory.h"


namespace Flags
{
    /* Constant Expressions: CPU Status Flags */
    constexpr U8 CARRY_FLAG                 = 0x01; /* BIT0: Carry Flag */
    constexpr U8 ZERO_FLAG                  = 0x02; /* BIT1: Zero Flag */
    constexpr U8 INTERRUPT_DISABLE_FLAG     = 0x04; /* BIT2: Interrupt Disabled Flag */
    constexpr U8 DECIMAL_MODE_FLAG          = 0x08; /* BIT3: Decimal Mode Flag (Holdover from 6502; NOT USED) */
    constexpr U8 BREAK_FLAG                 = 0x10; /* BIT4: Break Flag */
    constexpr U8 UNUSED_FLAG                = 0x20; /* BIT5: UNUSED */
    constexpr U8 OVERFLOW_FLAG              = 0x40; /* BIT6: Overflow Flag */
    constexpr U8 NEGATIVE_FLAG              = 0x80; /* BIT7: Negative Flag */
    /* Constant Expressions: Common Flag States */
    constexpr U8 RESET                  = 0x24;
    constexpr U8 SET                    = 0x01;
    constexpr U8 CLEAR                  = 0X00;
}


class RP2A03
{
    private:
        /* Registers */
        U16 PC;   /* 16-bit Programm Counter Register */
        U8 SP;    /* 8-bit Stack Pointer Register */
        U8 A;     /* 8-bit Accumulator Register */
        U8 X;     /* 8-bit Index Register */
        U8 Y;     /* 8-bit Index Register (can't be used with SP) */

        /* 8-bit CPU Status Register */
        U8 status;

        /* Connected memory bus */
        Bus* memBus;


        /* Addressing Mode enumeration */
        enum class AddrMode
        {
            absin, absol, accum, immed, impli, nivim,
            relat, xiabs, xizpg, xizpi, yiabs, yizpg,
            yizpi, zpage
        };

        /* Instruction Prototype */
        struct Instr_t
        {
            std::string mnumonic;               /* Mnumonic OpCode */
            AddrMode addrMode;                  /* Addressing Mode */
            void (RP2A03::*func)(U16 operand);  /* Instruction function pointer */
            U8 opCode;                          /* Operation Code (hex) */
            U8 length;                          /* Length of instruction in bytes */
            U8 cycles;                          /* Number of machine cycles */
            U16 operand;                        /* Operand obtained from applying addressing mode */
        };

        /* Instruction Vector */
        const std::array<RP2A03::Instr_t, 256> instrArray = {{
            {"BRK", AddrMode::impli, &RP2A03::BRK, 0x00, 1, 7, 0},
            {"ORA", AddrMode::xizpi, &RP2A03::ORA, 0x01, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x02, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x03, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x04, 0, 0, 0},
            {"ORA", AddrMode::zpage, &RP2A03::ORA, 0x05, 2, 3, 0},
            {"ASL", AddrMode::zpage, &RP2A03::ASL, 0x06, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x07, 0, 0, 0},
            {"PHP", AddrMode::impli, &RP2A03::PHP, 0x08, 1, 3, 0},
            {"ORA", AddrMode::immed, &RP2A03::ORA, 0x09, 2, 2, 0},
            {"ASL", AddrMode::accum, &RP2A03::ASL, 0x0A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0C, 0, 0, 0},
            {"ORA", AddrMode::absol, &RP2A03::ORA, 0x0D, 3, 4, 0},
            {"ASL", AddrMode::absol, &RP2A03::ASL, 0x0E, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x0F, 0, 0, 0},
            {"BPL", AddrMode::relat, &RP2A03::BPL, 0x10, 2, 2, 0},
            {"ORA", AddrMode::yizpi, &RP2A03::ORA, 0x11, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x12, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x13, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x14, 0, 0, 0},
            {"ORA", AddrMode::xizpg, &RP2A03::ORA, 0x15, 2, 4, 0},
            {"ASL", AddrMode::xizpg, &RP2A03::ASL, 0x16, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x17, 0, 0, 0},
            {"CLC", AddrMode::impli, &RP2A03::CLC, 0x18, 1, 2, 0},
            {"ORA", AddrMode::yiabs, &RP2A03::ORA, 0x19, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1A, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1C, 0, 0, 0},
            {"ORA", AddrMode::xiabs, &RP2A03::ORA, 0x1D, 3, 4, 0},
            {"ASL", AddrMode::xiabs, &RP2A03::ASL, 0x1E, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x1F, 0, 0, 0},
            {"JSR", AddrMode::absol, &RP2A03::JSR, 0x20, 3, 6, 0},
            {"AND", AddrMode::xizpi, &RP2A03::AND, 0x21, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x22, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x23, 0, 0, 0},
            {"BIT", AddrMode::zpage, &RP2A03::BIT, 0x24, 2, 3, 0},
            {"AND", AddrMode::zpage, &RP2A03::AND, 0x25, 2, 3, 0},
            {"ROL", AddrMode::zpage, &RP2A03::ROL, 0x26, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x27, 0, 0, 0},
            {"PLP", AddrMode::impli, &RP2A03::PLP, 0x28, 1, 4, 0},
            {"AND", AddrMode::immed, &RP2A03::AND, 0x29, 2, 2, 0},
            {"ROL", AddrMode::accum, &RP2A03::ROL, 0x2A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x2B, 0, 0, 0},
            {"BIT", AddrMode::absol, &RP2A03::BIT, 0x2C, 3, 4, 0},
            {"AND", AddrMode::absol, &RP2A03::AND, 0x2D, 3, 4, 0},
            {"ROL", AddrMode::absol, &RP2A03::ROL, 0x2E, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x2F, 0, 0, 0},
            {"BMI", AddrMode::relat, &RP2A03::BMI, 0x30, 2, 2, 0},
            {"AND", AddrMode::yizpi, &RP2A03::AND, 0x31, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x32, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x33, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x34, 0, 0, 0},
            {"AND", AddrMode::xizpg, &RP2A03::AND, 0x35, 2, 4, 0},
            {"ROL", AddrMode::xizpg, &RP2A03::ROL, 0x36, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x37, 0, 0, 0},
            {"SEC", AddrMode::impli, &RP2A03::SEC, 0x38, 1, 2, 0},
            {"AND", AddrMode::yiabs, &RP2A03::AND, 0x39, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3A, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3C, 0, 0, 0},
            {"AND", AddrMode::xiabs, &RP2A03::AND, 0x3D, 3, 4, 0},
            {"ROL", AddrMode::xiabs, &RP2A03::ROL, 0x3E, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x3F, 0, 0, 0},
            {"RTI", AddrMode::impli, &RP2A03::RTI, 0x40, 1, 6, 0},
            {"EOR", AddrMode::xizpi, &RP2A03::EOR, 0x41, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x42, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x43, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x44, 0, 0, 0},
            {"EOR", AddrMode::zpage, &RP2A03::EOR, 0x45, 2, 3, 0},
            {"LSR", AddrMode::zpage, &RP2A03::LSR, 0x46, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x47, 0, 0, 0},
            {"PHA", AddrMode::impli, &RP2A03::PHA, 0x48, 1, 3, 0},
            {"EOR", AddrMode::immed, &RP2A03::EOR, 0x49, 2, 2, 0},
            {"LSR", AddrMode::accum, &RP2A03::LSR, 0x4A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x4B, 0, 0, 0},
            {"JMP", AddrMode::absol, &RP2A03::JMP, 0x4C, 3, 3, 0},
            {"EOR", AddrMode::absol, &RP2A03::EOR, 0x4D, 3, 4, 0},
            {"LSR", AddrMode::absol, &RP2A03::LSR, 0x4E, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x4F, 0, 0, 0},
            {"BVC", AddrMode::relat, &RP2A03::BVC, 0x50, 2, 2, 0},
            {"EOR", AddrMode::yizpi, &RP2A03::EOR, 0x51, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x52, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x53, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x54, 0, 0, 0},
            {"EOR", AddrMode::xizpg, &RP2A03::EOR, 0x55, 2, 4, 0},
            {"LSR", AddrMode::xizpg, &RP2A03::LSR, 0x56, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x57, 0, 0, 0},
            {"CLI", AddrMode::impli, &RP2A03::CLI, 0x58, 1, 2, 0},
            {"EOR", AddrMode::yiabs, &RP2A03::EOR, 0x59, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5A, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5C, 0, 0, 0},
            {"EOR", AddrMode::xiabs, &RP2A03::EOR, 0x5D, 3, 4, 0},
            {"LSR", AddrMode::xiabs, &RP2A03::LSR, 0x5E, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x5F, 0, 0, 0},
            {"RTS", AddrMode::impli, &RP2A03::RTS, 0x60, 1, 6, 0},
            {"ADC", AddrMode::xizpi, &RP2A03::ADC, 0x61, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x62, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x63, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x64, 0, 0, 0},
            {"ADC", AddrMode::zpage, &RP2A03::ADC, 0x65, 2, 3, 0},
            {"ROR", AddrMode::zpage, &RP2A03::ROR, 0x66, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x67, 0, 0, 0},
            {"PLA", AddrMode::impli, &RP2A03::PLA, 0x68, 1, 4, 0},
            {"ADC", AddrMode::immed, &RP2A03::ADC, 0x69, 2, 2, 0},
            {"ROR", AddrMode::accum, &RP2A03::ROR, 0x6A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x6B, 0, 0, 0},
            {"JMP", AddrMode::absin, &RP2A03::JMP, 0x6C, 3, 5, 0},
            {"ADC", AddrMode::absol, &RP2A03::ADC, 0x6D, 3, 4, 0},
            {"ROR", AddrMode::absol, &RP2A03::ROR, 0x6E, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x6F, 0, 0, 0},
            {"BVS", AddrMode::relat, &RP2A03::BVS, 0x70, 2, 2, 0},
            {"ADC", AddrMode::yizpi, &RP2A03::ADC, 0x71, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x72, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x73, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x74, 0, 0, 0},
            {"ADC", AddrMode::xizpg, &RP2A03::ADC, 0x75, 2, 4, 0},
            {"ROR", AddrMode::xizpg, &RP2A03::ROR, 0x76, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x77, 0, 0, 0},
            {"SEI", AddrMode::impli, &RP2A03::SEI, 0x78, 1, 2, 0},
            {"ADC", AddrMode::yiabs, &RP2A03::ADC, 0x79, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7A, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7C, 0, 0, 0},
            {"ADC", AddrMode::xiabs, &RP2A03::ADC, 0x7D, 3, 4, 0},
            {"ROR", AddrMode::xiabs, &RP2A03::ROR, 0x7E, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x7F, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x80, 0, 0, 0},
            {"STA", AddrMode::xizpi, &RP2A03::STA, 0x81, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x82, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x83, 0, 0, 0},
            {"STY", AddrMode::zpage, &RP2A03::STY, 0x84, 2, 3, 0},
            {"STA", AddrMode::zpage, &RP2A03::STA, 0x85, 2, 3, 0},
            {"STX", AddrMode::zpage, &RP2A03::STX, 0x86, 2, 3, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x87, 0, 0, 0},
            {"DEY", AddrMode::impli, &RP2A03::DEY, 0x88, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x89, 0, 0, 0},
            {"TXA", AddrMode::impli, &RP2A03::TXA, 0x8A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x8B, 0, 0, 0},
            {"STY", AddrMode::absol, &RP2A03::STY, 0x8C, 3, 4, 0},
            {"STA", AddrMode::absol, &RP2A03::STA, 0x8D, 3, 4, 0},
            {"STX", AddrMode::absol, &RP2A03::STX, 0x8E, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x8F, 0, 0, 0},
            {"BCC", AddrMode::relat, &RP2A03::BCC, 0x90, 2, 2, 0},
            {"STA", AddrMode::yizpi, &RP2A03::STA, 0x91, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x92, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x93, 0, 0, 0},
            {"STY", AddrMode::xizpg, &RP2A03::STY, 0x94, 2, 4, 0},
            {"STA", AddrMode::xizpg, &RP2A03::STA, 0x95, 2, 4, 0},
            {"STX", AddrMode::yizpg, &RP2A03::STX, 0x96, 2, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x97, 0, 0, 0},
            {"TYA", AddrMode::impli, &RP2A03::TYA, 0x98, 1, 2, 0},
            {"STA", AddrMode::yiabs, &RP2A03::STA, 0x99, 3, 5, 0},
            {"TSX", AddrMode::impli, &RP2A03::TSX, 0x9A, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9B, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9C, 0, 0, 0},
            {"STA", AddrMode::xiabs, &RP2A03::STA, 0x9D, 3, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9E, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0x9F, 0, 0, 0},
            {"LDY", AddrMode::immed, &RP2A03::LDY, 0xA0, 2, 2, 0},
            {"LDA", AddrMode::xizpi, &RP2A03::LDA, 0xA1, 2, 6, 0},
            {"LDX", AddrMode::immed, &RP2A03::LDX, 0xA2, 2, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xA3, 0, 0, 0},
            {"LDY", AddrMode::zpage, &RP2A03::LDY, 0xA4, 2, 3, 0},
            {"LDA", AddrMode::zpage, &RP2A03::LDA, 0xA5, 2, 3, 0},
            {"LDX", AddrMode::zpage, &RP2A03::LDX, 0xA6, 2, 3, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xA7, 0, 0, 0},
            {"TAY", AddrMode::impli, &RP2A03::TAY, 0xA8, 1, 2, 0},
            {"LDA", AddrMode::immed, &RP2A03::LDA, 0xA9, 2, 2, 0},
            {"TAX", AddrMode::impli, &RP2A03::TAX, 0xAA, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xAB, 0, 0, 0},
            {"LDY", AddrMode::absol, &RP2A03::LDY, 0xAC, 3, 4, 0},
            {"LDA", AddrMode::absol, &RP2A03::LDA, 0xAD, 3, 4, 0},
            {"LDX", AddrMode::absol, &RP2A03::LDX, 0xAE, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xAF, 0, 0, 0},
            {"BCS", AddrMode::relat, &RP2A03::BCS, 0xB0, 2, 2, 0},
            {"LDA", AddrMode::yizpi, &RP2A03::LDA, 0xB1, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB2, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB3, 0, 0, 0},
            {"LDY", AddrMode::xizpg, &RP2A03::LDY, 0xB4, 2, 4, 0},
            {"LDA", AddrMode::xizpg, &RP2A03::LDA, 0xB5, 2, 4, 0},
            {"LDX", AddrMode::yizpg, &RP2A03::LDX, 0xB6, 2, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xB7, 0, 0, 0},
            {"CLV", AddrMode::impli, &RP2A03::CLV, 0xB8, 1, 2, 0},
            {"LDA", AddrMode::yiabs, &RP2A03::LDA, 0xB9, 3, 4, 0},
            {"TSX", AddrMode::impli, &RP2A03::TSX, 0xBA, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xBB, 0, 0, 0},
            {"LDY", AddrMode::xiabs, &RP2A03::LDY, 0xBC, 3, 4, 0},
            {"LDA", AddrMode::xiabs, &RP2A03::LDA, 0xBD, 3, 4, 0},
            {"LDX", AddrMode::yiabs, &RP2A03::LDX, 0xBE, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xBF, 0, 0, 0},
            {"CPY", AddrMode::immed, &RP2A03::CPY, 0xC0, 2, 2, 0},
            {"CMP", AddrMode::xizpi, &RP2A03::CMP, 0xC1, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC2, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC3, 0, 0, 0},
            {"CPY", AddrMode::zpage, &RP2A03::CPY, 0xC4, 2, 3, 0},
            {"CMP", AddrMode::zpage, &RP2A03::CMP, 0xC5, 2, 3, 0},
            {"DEC", AddrMode::zpage, &RP2A03::DEC, 0xC6, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xC7, 0, 0, 0},
            {"INY", AddrMode::impli, &RP2A03::INY, 0xC8, 1, 2, 0},
            {"CMP", AddrMode::immed, &RP2A03::CMP, 0xC9, 2, 2, 0},
            {"DEX", AddrMode::impli, &RP2A03::DEX, 0xCA, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xCB, 0, 0, 0},
            {"CPY", AddrMode::absol, &RP2A03::CPY, 0xCC, 3, 4, 0},
            {"CMP", AddrMode::absol, &RP2A03::CMP, 0xCD, 3, 4, 0},
            {"DEC", AddrMode::immed, &RP2A03::DEC, 0xCE, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xCF, 0, 0, 0},
            {"BNE", AddrMode::relat, &RP2A03::BNE, 0xD0, 2, 2, 0},
            {"CMP", AddrMode::yizpi, &RP2A03::CMP, 0xD1, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD2, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD3, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD4, 0, 0, 0},
            {"CMP", AddrMode::xizpg, &RP2A03::CMP, 0xD5, 2, 4, 0},
            {"DEC", AddrMode::xizpg, &RP2A03::DEC, 0xD6, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xD7, 0, 0, 0},
            {"CLD", AddrMode::impli, &RP2A03::CLD, 0xD8, 1, 2, 0},
            {"CMP", AddrMode::yiabs, &RP2A03::CMP, 0xD9, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDA, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDB, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDC, 0, 0, 0},
            {"CMP", AddrMode::xiabs, &RP2A03::CMP, 0xDD, 3, 4, 0},
            {"DEC", AddrMode::xiabs, &RP2A03::DEC, 0xDE, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xDF, 0, 0, 0},
            {"CPX", AddrMode::immed, &RP2A03::CPX, 0xE0, 2, 2, 0},
            {"SBC", AddrMode::xizpi, &RP2A03::SBC, 0xE1, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE2, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE3, 0, 0, 0},
            {"CPX", AddrMode::zpage, &RP2A03::CPX, 0xE4, 2, 3, 0},
            {"SBC", AddrMode::zpage, &RP2A03::SBC, 0xE5, 2, 3, 0},
            {"INC", AddrMode::zpage, &RP2A03::INC, 0xE6, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xE7, 0, 0, 0},
            {"INX", AddrMode::impli, &RP2A03::INX, 0xE8, 1, 2, 0},
            {"SBC", AddrMode::immed, &RP2A03::SBC, 0xE9, 2, 2, 0},
            {"NOP", AddrMode::impli, &RP2A03::NOP, 0xEA, 1, 2, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xEB, 0, 0, 0},
            {"CPX", AddrMode::absol, &RP2A03::CPX, 0xEC, 3, 4, 0},
            {"SBC", AddrMode::absol, &RP2A03::SBC, 0xED, 3, 4, 0},
            {"INC", AddrMode::absol, &RP2A03::INC, 0xEE, 3, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xEF, 0, 0, 0},
            {"BEQ", AddrMode::relat, &RP2A03::BEQ, 0xF0, 2, 2, 0},
            {"SBC", AddrMode::yizpi, &RP2A03::SBC, 0xF1, 2, 5, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF2, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF3, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF4, 0, 0, 0},
            {"SBC", AddrMode::xizpg, &RP2A03::SBC, 0xF5, 2, 4, 0},
            {"INC", AddrMode::xizpg, &RP2A03::INC, 0xF6, 2, 6, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xF7, 0, 0, 0},
            {"SED", AddrMode::impli, &RP2A03::SED, 0xF8, 1, 2, 0},
            {"SBC", AddrMode::yiabs, &RP2A03::SBC, 0xF9, 3, 4, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFA, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFB, 0, 0, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFC, 0, 0, 0},
            {"SBC", AddrMode::xiabs, &RP2A03::SBC, 0xFD, 3, 4, 0},
            {"INC", AddrMode::xiabs, &RP2A03::INC, 0xFE, 3, 7, 0},
            {"NII", AddrMode::nivim, &RP2A03::NII, 0xFF, 0, 0, 0},
        }};

        /* Instruction Emulation Functions */
        /* Instructons: Load/Store */
        void LDA(U16 operand); void LDX(U16 operand); void LDY(U16 operand); void STA(U16 operand); void STX(U16 operand); void STY(U16 operand);
        /* Instructons: Transfer */
        void TAX(U16 operand); void TAY(U16 operand); void TSX(U16 operand); void TXA(U16 operand); void TXS(U16 operand); void TYA(U16 operand);
        /* Instructons: Stack */
        void PHA(U16 operand); void PHP(U16 operand); void PLA(U16 operand); void PLP(U16 operand);
        /* Instructons: Shift */
        void ASL(U16 operand); void LSR(U16 operand); void ROL(U16 operand); void ROR(U16 operand);
        /* Instructons: Logic */
        void AND(U16 operand); void BIT(U16 operand); void EOR(U16 operand); void ORA(U16 operand);
        /* Instructons: Arithmetic */
        void ADC(U16 operand); void CMP(U16 operand); void CPX(U16 operand); void CPY(U16 operand); void SBC(U16 operand);
        /* Instructons: Increment/Decrement */
        void DEC(U16 operand); void DEX(U16 operand); void DEY(U16 operand); void INC(U16 operand); void INX(U16 operand); void INY(U16 operand);
        /* Instructons: Control Flow */
        void BRK(U16 operand); void JMP(U16 operand); void JSR(U16 operand); void RTI(U16 operand); void RTS(U16 operand);
        /* Instructons: Branch */
        void BCC(U16 operand); void BCS(U16 operand); void BEQ(U16 operand); void BMI(U16 operand); void BNE(U16 operand); void BPL(U16 operand); void BVC(U16 operand); void BVS(U16 operand);
        /* Instructons: Flags */
        void CLC(U16 operand); void CLD(U16 operand); void CLI(U16 operand); void CLV(U16 operand); void SEC(U16 operand); void SED(U16 operand); void SEI(U16 operand);
        /* Instructons: No Operation */
        void NOP(U16 operand);
        /* Instructons: NULL Index Instruction (NOT PART OF THE RP2A03 ABI) */
        void NII(U16 operand);
        
        /* Pseudo-pipeline member functions */
        U8 fetch(void);
        Instr_t decode(U8 opCode);
        void applyAddressingMode(Instr_t* instr);
        void executeInstruction(Instr_t* instr);

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
        inline U16 getPC(void) { return PC; }
        inline U8 getSP(void) { return SP; }
        inline U8 getA(void) { return A; }
        inline U8 getX(void) { return X; }
        inline U8 getY(void) { return Y; }
        inline U8 getStatus(void) { return status; }

        /* Modifiers */
        inline void setFlags(U8 flags){ status |= flags; }
        inline void clearFlags(U8 flags){ status &= ~flags; };
};


#endif /* RP2A03_H */