#include "../inc/rp2c02.h"

RP2C02::RP2C02()
{
    /* Set PPU memory */
    patternTables   = ((PatternTableMem_Typedef*) PPU_PTRN_TABLE0_BASE_ADDR);
    nameTables      = ((NameTableMem_Typedef*) PPU_NAME_TABLE0_BASE_ADDR);
    paletteTables   = ((PaletteMem_Typedef*) PPU_IMAGE_PALETTE_BASE_ADDR);
    mirrors         = ((MirrorMem_Typedef*) PPU_MIRROR2_BASE_ADDR);
}


RP2C02::~RP2C02()
{

}