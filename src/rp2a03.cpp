#include "../inc/rp2a03.h"


RP2A03::RP2A03()
{
    reset();
}

RP2A03::~RP2A03(){}


/**
 * @brief Executes a single loop of the CPU pipeline
 * 
 */
void RP2A03::CPU_Cycle(void)
{
    U8 opcode = fetch();
    Instr_t nextInstruction = decode(opcode);
    executeInstruction(&nextInstruction);
}


/**
 * @brief Applies the correct addressing mode to an instruction
 * 
 * @param instr Instruction to apply addressing mode on
 */
void RP2A03::applyAddressingMode(Instr_t* instr)
{
    // Addressing Mode jump table
    switch(instr->addrMode)
    {
        case AddrMode::absin:
        {
            /* In absolute indirect addressing, the second byte of the instruction contains the
               low order byte of a memory location. The high order byte of that memory location
               are contained in the third byte of the instruction. */

            U8 lowByte = memBus->readFromBus(PC + 1);
            U8 highByte = memBus->readFromBus(PC + 2);
            U16 effectiveAddress = (highByte << 8) | lowByte;

            U8 targetLowByte = memBus->readFromBus(effectiveAddress);
            U8 targetHighByte = memBus->readFromBus(effectiveAddress + 1);
            U16 targetAddress = (targetHighByte << 8) | targetLowByte;

            instr->operand = targetAddress; 
            break;
        }
        case AddrMode::absol:
        {
            /* In absolute addressing, the second byte of the instruction specifies the eight low
               order bits of the effective address while the third byte specifies the eight high
               order bits */

            U8 lowByte = memBus->readFromBus(PC + 1);
            U8 highByte = memBus->readFromBus(PC + 2);
            U16 effectiveAddress = (highByte << 8) | lowByte;
            instr->operand = effectiveAddress;
            break;
        }
        case AddrMode::accum:
        {
            /* In accumulator addressing, the one byte instruction implies operation on the accumulator (A)
               register. */

            instr->operand = A;
            break;
        }
        case AddrMode::immed:
        {
            /* In immediate addressing, the second byte of the instruction contains the operand, with no
               further memory addressing required. */

            U8 immediateValue = memBus->readFromBus(PC + 1);
            instr->operand = immediateValue;
            break;
        }
        case AddrMode::impli:
        {
            /* In implied addressing, the address containing the operand is implicitly stated in the opcode
               mnumonic. */
            
            instr->operand = 0;
            break;
        }
        case AddrMode::nivim:
        {
            instr->operand = 0;
            break;
        }
        case AddrMode::relat:
        {
            /* In relative addressing, the second byte of the instruction is an operand. This operand
               is an offset which is added to the program counter when the counter is set at the next
               instruction. The range of the offset is -128 - +127 (signed byte). */

            int8_t offset = static_cast<int8_t>(memBus->readFromBus(PC + 1));
            instr->operand = PC + offset;
            break;
        }
        case AddrMode::xiabs:
        {
            /* In X-indexed absolute addressing, the effective address is formed by adding the contents
             of the X register to the address contained in the second and third bytes of the instruction. */

            U8 lowByte = memBus->readFromBus(PC + 1);
            U8 highByte = memBus->readFromBus(PC + 2);
            U16 address = (highByte << 8) | lowByte;

            U16 effectiveAddress = address + X;
            instr->operand = effectiveAddress;
            break;
        }
        case AddrMode::xizpg:
        {
            /* In X-indexed zero page adressing, the effective address is calculated by adding the
               second byte to the contents of the X index (X) register. Since this is a form of zero
               page addressing, the content of the second byte references a location in zero page memory.
               No carry is added to the high order byte of memory and crossing a page boundary does not
               occur */

            U8 zeroPageAddress = memBus->readFromBus(PC + 1);
            U16 effectiveAddress = (zeroPageAddress + X) & 0xFF;

            // Ensure wrapping around zero page memory (0x0000 - 0x00FF)
            effectiveAddress &= 0xFF;

            instr->operand = effectiveAddress;
            break;
        }
        case AddrMode::xizpi:
        {
            /* In X-index zero page indirect addressing, the second byte of the instruction is added to
               the contents of the X index (X) register and discards the carry. The result of this addition
               points to a memory location on page zero which contains the low order byte of the effective
               address. The next memory location in page zero contains the high order byte of the effective
               address. Both memory locations specifying the effective address must be in page zero. */

            U8 zeroPageAddress = memBus->readFromBus(PC + 1);
            U16 effectiveAddress = zeroPageAddress + X;
            effectiveAddress &= 0xFF;

            U8 lowByte = memBus->readFromBus(effectiveAddress);
            U8 highByte = memBus->readFromBus((effectiveAddress + 1) & 0xFF);
            U16 finalAddress = (highByte << 8) | lowByte;

            instr->operand = finalAddress;
            break;
        }
        case AddrMode::yiabs:
        {
            /* In Y-indexed absolute addressing, the effective address is formed by adding the contents
               of the Y register to the address contained in the second and third bytes of the instruction. */

            U8 lowByte = memBus->readFromBus(PC + 1);
            U8 highByte = memBus->readFromBus(PC + 2);
            U16 address = (highByte << 8) | lowByte;

            U16 effectiveAddress = address + Y;
            instr->operand = effectiveAddress;
            break;
        }
        case AddrMode::yizpg:
        {
            /* In Y-indexed zero page adressing, the effective address is calculated by adding the
               second byte to the contents of the Y index (Y) register. Since this is a form of zero
               page addressing, the content of the second byte references a location in zero page memory.
               No carry is added to the high order byte of memory and crossing a page boundary does not
               occur */

            U8 zeroPageAddress = memBus->readFromBus(PC + 1);
            U16 effectiveAddress = zeroPageAddress + Y;

            // Ensure wrapping around zero page memory (0x0000 - 0x00FF)
            effectiveAddress &= 0xFF;
            
            instr->operand = effectiveAddress;
            break;
        }
        case AddrMode::yizpi:
        {
            /* In Y-index zero page indirect addressing, the second byte of the instruction is added to
               the contents of the Y index (Y) register and discards the carry. The result of this addition
               points to a memory location on page zero which contains the low order byte of the effective
               address. The next memory location in page zero contains the high order byte of the effective
               address. Both memory locations specifying the effective address must be in page zero. */

            U8 zeroPageAddress = memBus->readFromBus(PC + 1);
            U16 effectiveAddress = zeroPageAddress + Y;
            effectiveAddress &= 0xFF;

            U8 lowByte = memBus->readFromBus(effectiveAddress);
            U8 highByte = memBus->readFromBus((effectiveAddress + 1) & 0xFF);
            U16 finalAddress = (highByte << 8) | lowByte;

            instr->operand = finalAddress;
            break;
        }
        case AddrMode::zpage:
        {
            /* In zero page addressing, the second byte of the insturction contains the low order byte of a
               memory location. The high byte of the memory location is assumed to be zero (0x00) */

            U8 lowByte = memBus->readFromBus(PC + 1);

            // Explicitly set the high byte to 0x00
            U8 zeroPageAddress = 0x00FF & static_cast<U16>(lowByte);

            instr->operand = zeroPageAddress;
            break;
        }
        default: /* AddrMode::nivim */
        {
            /* Null Instruction Vector Index Mode: Invalid opcode */
            
            instr->operand = 0;
            break;
        }
    }
}


/**
 * @brief Fetches the next opcode byte from memory
 * 
 * @return opcode fetched from memory 
 */
U8 RP2A03::fetch(void)
{
    PC++;
    U16 instrAddr = PC + MemoryMap::MEM_PRG_ROM_LOWER_BASE_ADDR;
    return memBus->readFromBus(instrAddr);
}


/**
 * @brief Decodes the current instruction opcode
 * 
 * @details Indexes the instruction array to decode the opcode
 * 
 * @param opCode Used to index the instr array
 * 
 * @return Decoded instruction 
 */
RP2A03::Instr_t RP2A03::decode(U8 opCode)
{
    // Decode the opCode byte read from ROM
    Instr_t instruction = instrArray[opCode];

    // Apply the correct addressing mode on the current instruction
    applyAddressingMode(&instruction);

    return instruction;
}


/**
 * @brief Wrapper for executing instruction function pointers
 * 
 */
void RP2A03::executeInstruction(Instr_t* instr)
{
    (this->*(instr->func))(instr->operand);
}

/**
 * @brief Executes the CPU reset vector
 * 
 */
void RP2A03::reset()
{
    // Initialize status register
    status = Flags::RESET;

    // Initialize SP
    SP = (U8)MemoryMap::MEM_RAM_STACK_BASE_ADDR;

    // Initialize PC to the reset vector
    PC = MemoryMap::MEM_INT_RESET_BASE_ADDR;
}


void RP2A03::NMI(){}
void RP2A03::IRQ(){}


/******************************************************************
 *                        Instructions                            *
 ******************************************************************/

/**
 * @brief Load Accumulator with Memory
 * 
 * @param a Value to load into the accumulator 
 * @param b OperandB
 */
void RP2A03::LDA(U16 operand)
{
    // Load the value of the operand into the accumulator
    A = operand;

    U8 flags = Flags::CLEAR;

    // Raise status flags if needed    
    if(A == 0)
    {
        flags |= Flags::ZERO_FLAG; 
    }
    else if(A & Flags::NEGATIVE_FLAG)
    {
        flags |= Flags::NEGATIVE_FLAG;
    }

    status |= flags;
}

void RP2A03::LDX(U16 operand){ (void)operand; }
void RP2A03::LDY(U16 operand){ (void)operand; }
void RP2A03::STA(U16 operand){ (void)operand; }
void RP2A03::STX(U16 operand){ (void)operand; }
void RP2A03::STY(U16 operand){ (void)operand; }
void RP2A03::TAX(U16 operand){ (void)operand; }
void RP2A03::TAY(U16 operand){ (void)operand; }
void RP2A03::TSX(U16 operand){ (void)operand; }
void RP2A03::TXA(U16 operand){ (void)operand; }
void RP2A03::TXS(U16 operand){ (void)operand; }
void RP2A03::TYA(U16 operand){ (void)operand; }
void RP2A03::PHA(U16 operand){ (void)operand; }
void RP2A03::PHP(U16 operand){ (void)operand; }
void RP2A03::PLA(U16 operand){ (void)operand; }
void RP2A03::PLP(U16 operand){ (void)operand; }
void RP2A03::ASL(U16 operand){ (void)operand; }
void RP2A03::LSR(U16 operand){ (void)operand; }
void RP2A03::ROL(U16 operand){ (void)operand; }
void RP2A03::ROR(U16 operand){ (void)operand; }
void RP2A03::AND(U16 operand){ (void)operand; }
void RP2A03::BIT(U16 operand){ (void)operand; }
void RP2A03::EOR(U16 operand){ (void)operand; }
void RP2A03::ORA(U16 operand){ (void)operand; }
void RP2A03::ADC(U16 operand){ (void)operand; }
void RP2A03::CMP(U16 operand){ (void)operand; }
void RP2A03::CPX(U16 operand){ (void)operand; }
void RP2A03::CPY(U16 operand){ (void)operand; }
void RP2A03::SBC(U16 operand){ (void)operand; }
void RP2A03::DEC(U16 operand){ (void)operand; } 
void RP2A03::DEX(U16 operand){ (void)operand; } 
void RP2A03::DEY(U16 operand){ (void)operand; } 
void RP2A03::INC(U16 operand){ (void)operand; } 
void RP2A03::INX(U16 operand){ (void)operand; }
void RP2A03::INY(U16 operand){ (void)operand; }
void RP2A03::BRK(U16 operand){ (void)operand; }
void RP2A03::JMP(U16 operand){ (void)operand; }
void RP2A03::JSR(U16 operand){ (void)operand; }
void RP2A03::RTI(U16 operand){ (void)operand; }
void RP2A03::RTS(U16 operand){ (void)operand; }
void RP2A03::BCC(U16 operand){ (void)operand; }
void RP2A03::BCS(U16 operand){ (void)operand; }
void RP2A03::BEQ(U16 operand){ (void)operand; }
void RP2A03::BMI(U16 operand){ (void)operand; }
void RP2A03::BNE(U16 operand){ (void)operand; }
void RP2A03::BPL(U16 operand){ (void)operand; } 
void RP2A03::BVC(U16 operand){ (void)operand; }
void RP2A03::BVS(U16 operand){ (void)operand; }
void RP2A03::CLC(U16 operand){ (void)operand; }
void RP2A03::CLD(U16 operand){ (void)operand; }
void RP2A03::CLI(U16 operand){ (void)operand; }
void RP2A03::CLV(U16 operand){ (void)operand; }
void RP2A03::SEC(U16 operand){ (void)operand; }
void RP2A03::SED(U16 operand){ (void)operand; }
void RP2A03::SEI(U16 operand){ (void)operand; }
void RP2A03::NOP(U16 operand){ (void)operand; }
void RP2A03::NII(U16 operand){ (void)operand; }
