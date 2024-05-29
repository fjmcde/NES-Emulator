#include "../inc/bus.h"

Bus::Bus()
{
    // The bus owns the NESMemory object
    nes_memory = std::make_unique<NESMemory>();
}

Bus::~Bus(){}

void Bus::writeToBus(U16 addr, U8 data){ (void)addr; (void)data; }
U8 Bus::readFromBus(U16 addr){ (void)addr; return 0; }