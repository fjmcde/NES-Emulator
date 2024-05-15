#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <array>
#include "global.h"


/* Memory Map Definitions: CPU RAM */
#define MEM_BASE_ADDR                   (U16)(0x0000)
#define MEM_RAM_ZERO_PAGE_BASE_ADDR     (U16)(0x0000)
#define MEM_RAM_STACK_BASE_ADDR         (U16)(0x0100)
#define MEM_RAM_RAM_BASE_ADDR           (U16)(0x0200)
#define MEM_RAM_MIRROR_BASE_ADDR        (U16)(0x0800)
/* Memory Map Definitions: I/O */
#define MEM_IO_BASE_ADDR                (U16)(0x2000)
#define MEM_IO_REGISTER_1_BASE_ADDR     (U16)(0x2000)
#define MEM_IO_MIRROR_BASE_ADDR         (U16)(0x2008)
#define MEM_IO_REGISTER_2_BASE_ADDR     (U16)(0x4000)
/* Memory Map Definitions: ROM */
#define MEM_ROM_EXP_BASE_ADDR           (U16)(0x4020)
/* Memory Map Definitions: SRAM */
#define MEM_SRAM_BASE_ADDR              (U16)(0x6000)
/* Memory Map Definitions: PRG-ROM */
#define MEM_PRG_ROM_LOWER_BASE_ADDR     (U16)(0x8000)
#define MEM_PRG_ROM_UPPER_BASE_ADDR     (U16)(0xC000)
#define MEM_END_OF_MEMORY_ADDR          (U16)(0xFFFF)


typedef struct
{
    std::array<U8, 256>*    zero_page;   /* 0x0000 - 0x00FF */
    std::array<U8, 256>*    stack;       /* 0x0100 - 0x01FF */
    std::array<U8, 1536>*   ram;        /* 0x0200 - 0x07FF */
}RAM_TypeDef;


typedef struct
{
    U8 io_registers1;                   /* 0x2000 - 0x2007 */
    std::array<U8, 32>  io_registers2;  /* 0x4000 - 0x401F */
}IO_TypeDef;


typedef struct
{
    std::array<U8, 8160> expansion_rom; /* 0x4020 - 0x5FFF */
}ROM_TypeDef;


typedef struct
{
    std::array<U8, 8192> sram;          /* 0x6000 - 0x7FFF */
}SRAM_TypeDef;


/* PRG: Program memory for the game */
typedef struct
{
    std::array<U8, 16384> prg_rom_lower;  /* 0x8000 - 0xBFFF */
    std::array<U8, 16384> prg_rom_upper;  /* 0xC000 - 0xFFFF */
}PRG_ROM_TypeDef;


#define MEMORY      ((RAM_TypeDef*) MEM_BASE_ADDR)
#define IO          ((IO_TypeDef*) MEM_IO_BASE_ADDR)
#define ROM         ((ROM_TypeDef*) MEM_ROM_EXP_BASE_ADDR)
#define SRAM        ((SRAM_TypeDef*) MEM_SRAM_BASE_ADDR)
#define PRGRAM      ((PRG_ROM_TypeDef*) MEM_PRG_ROM_LOWER_BASE_ADDR)


#endif /* MEMORY_MAP_H */