#ifndef BUS_H
#define BUS_H

#include "global.h"

class Bus
{
    private:

    public:
        Bus();
        ~Bus();

        void writeToBus(U16 addr, U8 data);
        U8 readFromBus(U16 addr);
};


#endif /* BUS_H */