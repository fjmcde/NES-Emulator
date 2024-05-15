#ifndef BUS_H
#define BUS_H

#include "global.h"
#include "rp2a03.h"

class Bus
{
    private:
        RP2A03 cpu;

    public:
        Bus();
        ~Bus();

        void writeToBus(U16 addr, U8 data);
        void readFromBus(U16 addr, U8 data);
};


#endif /* BUS_H */