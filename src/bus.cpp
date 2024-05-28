#include "../inc/bus.h"

Bus::Bus(){}
Bus::~Bus(){}

void Bus::writeToBus(U16 addr, U8 data){ (void)addr; (void)data; }
U8 Bus::readFromBus(U16 addr){ (void)addr; return 0; }