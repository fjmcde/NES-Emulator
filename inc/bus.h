#ifndef BUS_H
#define BUS_H

#include <memory>
#include "global.h"
#include "nesmemory.h"

class Bus
{
    private:
        std::unique_ptr<NESMemory> nes_memory;

    public:
        Bus();
        ~Bus();

        void writeToBus(U16 addr, U8 data);
        U8 readFromBus(U16 addr);
};


#endif /* BUS_H */