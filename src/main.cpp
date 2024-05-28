#include "../inc/main.h"


int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    RP2A03 cpu;

    cpu.reset();

    while(1)
    {
        cpu.CPU_Cycle();
    }
}