#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include <array>
#include "global.h"

namespace MemoryMap
{
    /* Memory Map Definitions: CPU RAM */
    constexpr U16 MEM_BASE_ADDR                     = 0x0000;
    constexpr U16 MEM_RAM_ZERO_PAGE_BASE_ADDR       = 0x0000;
    constexpr U16 MEM_RAM_STACK_BASE_ADDR           = 0x0100;
    constexpr U16 MEM_RAM_RAM_BASE_ADDR             = 0x0200;
    constexpr U16 MEM_RAM_MIRROR_BASE_ADDR          = 0x0800;
    /* Memory Map Definitions: I/O */
    constexpr U16 MEM_IO_BASE_ADDR                  = 0x2000;
    constexpr U16 MEM_IO_REGISTER_1_BASE_ADDR       = 0x2000;
    constexpr U16 MEM_IO_MIRROR_BASE_ADDR           = 0x2008;
    constexpr U16 MEM_IO_REGISTER_2_BASE_ADDR       = 0x4000;
    /* Memory Map Definitions: ROM */
    constexpr U16 MEM_ROM_EXP_BASE_ADDR             = 0x4020;
    /* Memory Map Definitions: SRAM */
    constexpr U16 MEM_SRAM_BASE_ADDR                = 0x6000;
    /* Memory Map Definitions: PRG-ROM */
    constexpr U16 MEM_PRG_ROM_LOWER_BASE_ADDR       = 0x8000;
    constexpr U16 MEM_PRG_ROM_UPPER_BASE_ADDR       = 0xC000;
    /* Memory Map Definitions: Interrupt Vectors */
    constexpr U16 MEM_INT_NMI_BASE_ADDR             = 0xFFFA;
    constexpr U16 MEM_INT_RESET_BASE_ADDR           = 0xFFFC;
    constexpr U16 MEM_INT_IRQ_BASE_ADDR             = 0xFFFE;
    constexpr U16 MEM_END_OF_MEMORY_ADDR            = 0xFFFF;
}


class NESMemory
{
    private:
        struct RAM_Typedef
        {
            std::array<U8, 256> zero_page;  /* 0x0000 - 0x00FF */
            std::array<U8, 256> stack;      /* 0x0100 - 0x01FF */
            std::array<U8, 1536> ram;       /* 0x0200 - 0x07FF */
        }ram;


        struct IO_Typedef
        {
            std::array<U8, 8> io_registers1;    /* 0x2000 - 0x2007 */
            std::array<U8, 32>  io_registers2;  /* 0x4000 - 0x401F */
        }io;


        struct ROM_Typedef
        {
            std::array<U8, 8160> expansion_rom; /* 0x4020 - 0x5FFF */
        }rom;


        struct SRMA_Typedef
        {
            std::array<U8, 8192> sram;          /* 0x6000 - 0x7FFF */
        }sram;


        /* PRG: Program memory for the game */
        struct PRGROM_Typedef
        {
            std::array<U8, 16384> prg_rom_lower;  /* 0x8000 - 0xBFFF */
            std::array<U8, 16384> prg_rom_upper;  /* 0xC000 - 0xFFFF */
        }prg_rom;

    public:
        NESMemory();
        ~NESMemory();        
};





#endif /* NES_MEMORY_H */