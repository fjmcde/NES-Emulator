#ifndef RP2C02_H
#define RP2C02_H

#include <array>
#include "global.h"

/* PPU Memory Map Definitions */
#define PPU_MEM_BASE_ADDR                   (U16)(0X0000)
#define PPU_PTRN_TABLE0_BASE_ADDR           (U16)(0x0000)
#define PPU_PTRN_TABLE1_BASE_ADDR           (U16)(0x1000)
#define PPU_NAME_TABLE0_BASE_ADDR           (U16)(0x2000)
#define PPU_ATTR_TABLE0_BASE_ADDR           (U16)(0x23C0)
#define PPU_NAME_TABLE1_BASE_ADDR           (U16)(0x2400)
#define PPU_ATTR_TABLE1_BASE_ADDR           (U16)(0x27C0)
#define PPU_NAME_TABLE2_BASE_ADDR           (U16)(0x2800)
#define PPU_ATTR_TABLE2_BASE_ADDR           (U16)(0x2BC0)
#define PPU_NAME_TABLE3_BASE_ADDR           (U16)(0x2C00)
#define PPU_ATTR_TABLE3_BASE_ADDR           (U16)(0x2FC0)
#define PPU_MIRROR1_BASE_AADR               (U16)(0x3000)
#define PPU_IMAGE_PALETTE_BASE_ADDR         (U16)(0x3F00)
#define PPU_SPRITE_PALETTE_BASE_ADDR        (U16)(0x3F10)
#define PPU_MIRROR2_BASE_ADDR               (U16)(0x3F20)
#define PPU_MIRROR3_BASE_ADDR               (U16)(0x4000)


class RP2C02
{
    private:
        struct PatternTableMem_Typedef
        {
            std::array<U8, 4096> patternTable0;     /* 0x0000 - 0x0FFF */
            std::array<U8, 4096> patternTable1;     /* 0x1000 - 0x1FFF */
        }*patternTables;


        struct NameTableMem_Typedef
        {
            std::array<U8, 960> nameTable0;         /* 0x2000 - 0x23BF */
            std::array<U8, 64> attrTable0;          /* 0x23CO - 0x23FF */
            std::array<U8, 960> nameTable1;         /* 0x2400 - 0x27BF */
            std::array<U8, 64> attrTable1;          /* 0x27C0 - 0x27FF */
            std::array<U8, 960> nameTable2;         /* 0x2800 - 0x2BBF */
            std::array<U8, 64> attrTable2;          /* 0x2BC0 - 0x2BFF */
            std::array<U8, 960> nameTable3;         /* 0x2C00 - 0x2FBF */
            std::array<U8, 64> attrTable3;          /* 0x2FC0 - 0x2FFF */
            std::array<U8, 3840> mirrors;           /* 0x3000 - 0x3EFF */
        }*nameTables;


        struct PaletteMem_Typedef
        {
            std::array<U8, 16> imagePalette;        /* 0x3F00 - 0x3F0F */
            std::array<U8, 16> spritePalette;       /* 0x3F10 - 0x3F1F */
            std::array<U8, 224> mirrors;            /* 0x3f20 - 0x4000 */
        }*paletteTables;


        struct MirrorMem_Typedef
        {
            std::array<U8, 24576> mirrors;          /* 0x4000 - 0x9FFF */
        }*mirrors;

    public:
        RP2C02();
        ~RP2C02();
};


#endif /* RP2C02 */