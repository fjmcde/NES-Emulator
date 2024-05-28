#include "../inc/rp2a03.h"


RP2A03::RP2A03()
{
    reset();
}

RP2A03::~RP2A03(){}


void RP2A03::CPU_Cycle(void)
{
    // Fetch the next instruction from program ROM
    U8 nextInstruction = fetch();

    // Decode the instruction
    decode(nextInstruction);

    // Execute ins
    executeInstruction();
}


void RP2A03::applyAddressingMode(void)
{
    // Addressing Mode jump table
    switch(currentInstruction.addrMode)
    {
        case AddrMode::impli:
            /* In implied addressing, the address containing the operand is 
               implicitly stated in the opcode mnumonic. */

            currentInstruction.firstOperand = nullptr;
            currentInstruction.secondOperand = nullptr;
            break;
        case AddrMode::accum:
            /* In accumulator addressing, the one byte instruction implies
               operation on the accumulator (A) register. */

            *currentInstruction.firstOperand = A;
            currentInstruction.secondOperand = nullptr;
            break;
        case AddrMode::immed:
            /* In immediate addressing, the second byte of the instruction
               contains the operand, with no further memory addressing required. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = nullptr;
            break;
        case AddrMode::absol:
            /* In absolute addressing, the second byte of the instruction
               specifies the eight low order bits of the effective address
               while the third byte specifies the eight high order bits */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            *currentInstruction.secondOperand = memBus->readFromBus(PC + 2);
            break;
        case AddrMode::xiabs:
            /* In X-indexed absolute addressing, the effective address is
               formed by adding the contents of the X register to the address
               contained in the second and third bytes of the instruction. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            *currentInstruction.secondOperand = memBus->readFromBus(PC + 2);
            break;
        case AddrMode::yiabs:
            /* In Y-indexed absolute addressing, the effective address is
               formed by adding the contents of the Y register to the address
               contained in the second and third bytes of the instruction. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            *currentInstruction.secondOperand = memBus->readFromBus(PC + 2);
            break;
        case AddrMode::absin:
            /* In absolute indirect addressing, the second byte of the instruction
               contains the low order byte of a memory location. The high order byte
               of that memory location are contained in the third byte of the instruction. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            *currentInstruction.secondOperand = memBus->readFromBus(PC + 2);
            break;
        case AddrMode::zpage:
            /* In zero page addressing, the second byte of the insturction contains
               the low order byte of a memory location. The high byte of the memory location
               is assumed to be zero (0x00) */
            
            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            *currentInstruction.secondOperand = 0x00;
            break;
        case AddrMode::xizpg:
            /* In X-indexed zero page adressing, the effective address is calculated by
               adding the second byte to the contents of the X index (X) register. Since
               this is a form of zero page addressing, the content of the second byte 
               references a location in zero page memory. No carry is added to the high
               order byte of memory and crossing a page boundary does not occur */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = &X;
            break;
        case AddrMode::yizpg:
            /* In Y-indexed zero page adressing, the effective address is calculated by
               adding the second byte to the contents of the Y index (Y) register. Since
               this is a form of zero page addressing, the content of the second byte 
               references a location in zero page memory. No carry is added to the high
               order byte of memory and crossing a page boundary does not occur */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = nullptr;
            break;
        case AddrMode::xizpi:
            /* In X-index zero page indirect addressing, the second byte of the instruction
               is added to the contents of the X index (X) register and discards the carry.
               The result of this addition points to a memory location on page zero which contains
               the low order byte of the effective address. The next memory location in page zero
               contains the high order byte of the effective address. Both memory locations specifying
               the effective address must be in page zero. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = &X;
            break;
        case AddrMode::yizpi:
            /* In Y-index zero page indirect addressing, the second byte of the instruction
               is added to the contents of the Y index (Y) register and discards the carry.
               The result of this addition points to a memory location on page zero which contains
               the low order byte of the effective address. The next memory location in page zero
               contains the high order byte of the effective address. Both memory locations specifying
               the effective address must be in page zero. */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = &Y;
            break;
        case AddrMode::relat:
            /* In relative addressing, the second byte of the instruction is an operand. This operand
               is an offset which is added to the program counter when the counter is set at the next
               instruction. The range of the offset is -128 - +127 (signed byte). */

            *currentInstruction.firstOperand = memBus->readFromBus(PC + 1);
            currentInstruction.secondOperand = nullptr;
            break;
        default: /* AddrMode::nivim */
            /* Null Instruction Vector Index Mode: Invalid opcode */
            
            currentInstruction.firstOperand = nullptr;
            currentInstruction.secondOperand = nullptr;
            break;
    }
}


U8 RP2A03::fetch(void)
{
    PC++;
    // U16 instrAddr = 

    return memBus->readFromBus(PC);
}


void RP2A03::decode(U8 opCode)
{
    // Decode the opCode byte read from ROM
    currentInstruction = instr[opCode];

    // Apply the correct addressing mode on the current instruction
    applyAddressingMode();
}


void RP2A03::executeInstruction(void)
{
    (this->*currentInstruction.func)(*currentInstruction.firstOperand, *currentInstruction.secondOperand);
}

void RP2A03::reset()
{
    // Initialize status register
    status = { 0, 0, 1, 0, 0, 1, 0, 0 };

    // Initialize SP
    SP = (U8)MEM_RAM_STACK_BASE_ADDR;

    // Initialize PC to the reset vector
    PC = MEM_INT_RESET_BASE_ADDR;
}

void RP2A03::NMI(){}
void RP2A03::IRQ(){}


void RP2A03::setFlag(U8 flags)
{
    if(flags & carryFlag){ status.C = 0x1; }
    if(flags & zeroFlag){ status.Z = 0x1; }
    if(flags & interruptDisableFlag){ status.I = 0x1; }
    if(flags & decimalModeFlag){ status.D = 0x1; }
    if(flags & breakFlag){ status.B = 0x1; }
    if(flags & unusedFlag){ status.UNUSED = 0x1; }
    if(flags & overflowFlag){ status.V = 0x1; }
    if(flags & negativeFlag){ status.N = 0x1; }
}


/******************************************************************
 *                        Instructions                            *
 ******************************************************************/

void RP2A03::LDA(U8 a, U8 b)
{
    (void)a; (void)b;

    // Raise flags if needed
    U8 flags = 0x0;
    if(A == 0){ flags |= zeroFlag; }
    else if(A & 0x80){ flags |= negativeFlag; }

    setFlag(flags);
}

void RP2A03::LDX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::LDY(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::STA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::STX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::STY(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TAX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TAY(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TSX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TXA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TXS(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::TYA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::PHA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::PHP(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::PLA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::PLP(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::ASL(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::LSR(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::ROL(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::ROR(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::AND(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BIT(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::EOR(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::ORA(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::ADC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CMP(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CPX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CPY(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::SBC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::DEC(U8 a, U8 b){ (void)a; (void)b; } 
void RP2A03::DEX(U8 a, U8 b){ (void)a; (void)b; } 
void RP2A03::DEY(U8 a, U8 b){ (void)a; (void)b; } 
void RP2A03::INC(U8 a, U8 b){ (void)a; (void)b; } 
void RP2A03::INX(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::INY(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BRK(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::JMP(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::JSR(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::RTI(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::RTS(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BCC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BCS(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BEQ(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BMI(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BNE(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BPL(U8 a, U8 b){ (void)a; (void)b; } 
void RP2A03::BVC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::BVS(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CLC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CLD(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CLI(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::CLV(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::SEC(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::SED(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::SEI(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::NOP(U8 a, U8 b){ (void)a; (void)b; }
void RP2A03::NII(U8 a, U8 b){ (void)a; (void)b; }
