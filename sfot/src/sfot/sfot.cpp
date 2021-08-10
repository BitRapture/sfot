#include "sfot.h"

#include <iostream>

u16 sfot::AM_Abs(sfotmem& _mem)
{
	// Get address (little-endian) from next two bytes
	u16 addr = _mem[r_PC]; ++r_PC;
	addr |= (_mem[r_PC] << 8); ++r_PC;
	return addr;
}
u16 sfot::AM_AbsX(sfotmem& _mem)
{
	// Get address (little-endian) from next two bytes, offset by X
	u16 addr = _mem[r_PC]; ++r_PC;
	addr |= (_mem[r_PC] << 8); ++r_PC;
	addr += r_X;
	return addr;
}
u16 sfot::AM_AbsY(sfotmem& _mem)
{
	// Get address (little-endian) from next two bytes, offset by Y
	u16 addr = _mem[r_PC]; ++r_PC;
	addr |= (_mem[r_PC] << 8); ++r_PC;
	addr += r_Y;
	return addr;
}
u16 sfot::AM_Ind(sfotmem& _mem)
{
	// Get address indirectly from next two bytes
	u16 addr = _mem[r_PC]; ++r_PC;
	addr |= (_mem[r_PC] << 8); ++r_PC;
	addr = _mem[addr]; 
	return addr;
}
u16 sfot::AM_XInd(sfotmem& _mem)
{
	// Get address from the zero-page + r_X with wrap around
	u8 zpg = (u8)(_mem[r_PC] + r_X); ++r_PC;
	return (u16)(_mem[zpg] | (_mem[(u8)(zpg + 1)] << 8));
}
u16 sfot::AM_IndY(sfotmem& _mem)
{
	// Get address from the _mem(zero-page) + r_Y
	u16 zpg = _mem[r_PC]; ++r_PC;
	return (u16)(_mem[zpg] | (_mem[zpg + 1] << 8) + r_Y);
}
u16 sfot::AM_Rel(sfotmem& _mem)
{
	// For branches, next byte is a signed offset for r_PC
	u16 addr = (s8)(_mem[r_PC]); ++r_PC;
	addr += r_PC;
	return addr;
}
u16 sfot::AM_Zpg(sfotmem& _mem)
{
	// Zero-page addressing
	return _mem[r_PC++];
}
u16 sfot::AM_ZpgX(sfotmem& _mem)
{
	// Zero-page indirect addressing r_X
	return (u8)(_mem[r_PC++] + r_X);
}
u16 sfot::AM_ZpgY(sfotmem& _mem)
{
	// Zero-page indirect addressing r_Y
	return (u8)(_mem[r_PC++] + r_Y);
}

void sfot::O_LDA(u16& _addr, sfotmem& _mem)
{
	// Load accumulator
	r_A = _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | ((r_A >> 7) << (u8)r_SRSs::N) | (u8)r_SRS::S;
}
void sfot::O_LDX(u16& _addr, sfotmem& _mem)
{
	// Load X register
	r_X = _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_X) << (u8)r_SRSs::Z) | ((r_X >> 7) << (u8)r_SRSs::N) | (u8)r_SRS::S;
}
void sfot::O_LDY(u16& _addr, sfotmem& _mem)
{
	// Load Y register
	r_Y = _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_Y) << (u8)r_SRSs::Z) | ((r_Y >> 7) << (u8)r_SRSs::N) | (u8)r_SRS::S;
}
void sfot::O_STA(u16& _addr, sfotmem& _mem)
{
	// Send accumulator contents to memory
	_mem.Set(_addr, r_A);
}
void sfot::O_STX(u16& _addr, sfotmem& _mem)
{
	// Send X register contents to memory
	_mem.Set(_addr, r_X);
}
void sfot::O_STY(u16& _addr, sfotmem& _mem)
{
	// Send Y register contents to memory
	_mem.Set(_addr, r_Y);
}
void sfot::O_TAX(u16& _addr, sfotmem& _mem)
{
	// Transfer accumulator to X register
	r_X = r_A;
	// Set appropriate status flags
	r_SR = ((!r_X) << (u8)r_SRSs::Z) | (r_X & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_TAY(u16& _addr, sfotmem& _mem)
{
	// Transfer accumulator to Y register
	r_Y = r_A;
	// Set appropriate status flags
	r_SR = ((!r_Y) << (u8)r_SRSs::Z) | (r_Y & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_TXA(u16& _addr, sfotmem& _mem)
{
	// Transfer X register to accumulator
	r_A = r_X;
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_TYA(u16& _addr, sfotmem& _mem)
{
	// Transfer Y register to accumulator
	r_A = r_Y;
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_TSX(u16& _addr, sfotmem& _mem)
{
	// Transfer stack pointer to X register
	r_X = r_S;
	// Set appropriate status flags
	r_SR = ((!r_X) << (u8)r_SRSs::Z) | (r_X & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_TXS(u16& _addr, sfotmem& _mem)
{
	// Transfer X register to stack pointer
	r_S = r_X;
}
void sfot::O_PHA(u16& _addr, sfotmem& _mem)
{
	// Push accumulator to the stack
	u16 addr = 0x0100 | r_S; --r_S;
	_mem.Set(addr, r_A);
}
void sfot::O_PHP(u16& _addr, sfotmem& _mem)
{
	// Push status register to the stack
	u16 addr = 0x0100 | r_S; --r_S;
	_mem.Set(addr, r_SR);
}
void sfot::O_PLA(u16& _addr, sfotmem& _mem)
{
	// Pop top from stack to accumulator
	u16 addr = 0x0100 | ++r_S;
	r_A = _mem[addr];
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_PLP(u16& _addr, sfotmem& _mem)
{
	// Pop top from stack to status register
	u16 addr = 0x0100 | ++r_S;
	r_SR = _mem[addr] | (u8)r_SRS::S;
}
void sfot::O_AND(u16& _addr, sfotmem& _mem)
{
	// Logical AND on accumulator and memory
	r_A &= _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_EOR(u16& _addr, sfotmem& _mem)
{
	// Exclusive OR on accumulator and memory
	r_A ^= _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_ORA(u16& _addr, sfotmem& _mem)
{
	// Inclusive OR on accumulator and memory
	r_A |= _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_BIT(u16& _addr, sfotmem& _mem)
{
	// Bit test on accumulator and memory
	u8 test = r_A & _mem[_addr];
	// Set appropriate status flags
	r_SR = ((!test) << (u8)r_SRSs::Z) | (test & (u8)r_SRS::O) | (test & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_ADC(u16& _addr, sfotmem& _mem)
{
	// Add with carry (accumulator)
	u8 operand = _mem[_addr];
	u16 adc = r_A + operand + (r_SR & (u8)r_SRS::C);
	switch (r_SR & (u8)r_SRS::D)
	{
	case (u8)r_SRS::D: // Decimal mode
	{
		/* to-do: implement decimal mode */
	}
	default: // Binary mode
		// Set appropriate status flags
		r_SR = ((~(r_A ^ operand) & (r_A ^ adc) & 0x80) >> 1) | (adc > 0xff) | (adc & (u8)r_SRS::N) | ((!adc) << (u8)r_SRSs::Z) | (u8)r_SRS::S;
		// Set accumulator to sum
		r_A = adc & 0xff;
		break;
	}
}
void sfot::O_SBC(u16& _addr, sfotmem& _mem)
{
	// Subtract with carry (accumulator)
	// Invert the bits, little sneaky memory manip ;)
	_mem.Set(_addr, ~_mem[_addr]);
	sfot::O_ADC(_addr, _mem);
	_mem.Set(_addr, ~_mem[_addr]);
}
void sfot::O_CMP(u16& _addr, sfotmem& _mem)
{
	// Compare accumulator with memory
	u8 operand = _mem[_addr];
	// Set appropriate status flags
	r_SR = (r_A >= operand) | ((r_A == operand) << (u8)r_SRSs::Z) | ((u8)(r_A - operand) & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_CPX(u16& _addr, sfotmem& _mem)
{
	// Compare X register with memory
	u8 operand = _mem[_addr];
	// Set appropriate status flags
	r_SR = (r_X >= operand) | ((r_X == operand) << (u8)r_SRSs::Z) | ((u8)(r_X - operand) & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_CPY(u16& _addr, sfotmem& _mem)
{
	// Compare Y register with memory
	u8 operand = _mem[_addr];
	// Set appropriate status flags
	r_SR = (r_Y >= operand) | ((r_Y == operand) << (u8)r_SRSs::Z) | ((u8)(r_Y - operand) & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_INC(u16& _addr, sfotmem& _mem)
{
	// Increment value at memory address
	u8 operand = _mem[_addr]; ++operand;
	_mem.Set(_addr, operand);
	// Set appropriate status flags
	r_SR = ((!operand) << (u8)r_SRSs::Z) | (operand & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_INX(u16& _addr, sfotmem& _mem)
{
	// Increment value in X register
	++r_X;
	// Set appropriate status flags
	r_SR = ((!r_X) << (u8)r_SRSs::Z) | (r_X & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_INY(u16& _addr, sfotmem& _mem)
{
	// Increment value in Y register
	++r_Y;
	// Set appropriate status flags
	r_SR = ((!r_Y) << (u8)r_SRSs::Z) | (r_Y & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_DEC(u16& _addr, sfotmem& _mem)
{
	// Decrement value at memory address
	u8 operand = _mem[_addr]; --operand;
	_mem.Set(_addr, operand);
	// Set appropriate status flags
	r_SR = ((!operand) << (u8)r_SRSs::Z) | (operand & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_DEX(u16& _addr, sfotmem& _mem)
{
	// Decrement value in X register
	--r_X;
	// Set appropriate status flags
	r_SR = ((!r_X) << (u8)r_SRSs::Z) | (r_X & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_DEY(u16& _addr, sfotmem& _mem)
{
	// Decrement value in Y register
	--r_Y;
	// Set appropriate status flags
	r_SR = ((!r_Y) << (u8)r_SRSs::Z) | (r_Y & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_ASL(u16& _addr, sfotmem& _mem)
{
	// Arithmetic shift left (memory)
	u8 operand = _mem[_addr];
	// Set carry
	r_SR = (operand & 0x80) >> 7; operand <<= 1;
	_mem.Set(_addr, operand);
	// Set appropriate status flags
	r_SR |= ((!operand) << (u8)r_SRSs::Z) | (operand & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_ASL_A(u16& _addr, sfotmem& _mem)
{
	// Arithmetic shift left (accumulator)
	// Set carry
	r_SR = (r_A & 0x80) >> 7; r_A <<= 1;
	// Set appropriate status flags
	r_SR |= ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_LSR(u16& _addr, sfotmem& _mem)
{
	// Logical shift right (memory)
	u8 operand = _mem[_addr];
	// Set carry
	r_SR = (operand & 0x01); operand >>= 1;
	_mem.Set(_addr, operand);
	// Set appropriate status flags
	r_SR |= ((!operand) << (u8)r_SRSs::Z) | (operand & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_LSR_A(u16& _addr, sfotmem& _mem)
{
	// Logical shift right (accumulator)
	// Set carry
	r_SR = (r_A & 0x01); r_A >>= 1;
	// Set appropriate status flags
	r_SR |= ((!r_A) << (u8)r_SRSs::Z) | (r_A & (u8)r_SRS::N) | (u8)r_SRS::S;
}
void sfot::O_ROL(u16& _addr, sfotmem& _mem)
{
	// Rotate left (memory)
	u8 operand = _mem[_addr];
	u8 oldCarry = r_SR & (u8)r_SRS::C;
	// Set carry
	r_SR = (operand & 0x80) >> 7; operand <<= 1;
	operand |= oldCarry;
	// Set appropriate status flags
	r_SR |= ((!operand) << (u8)r_SRSs::Z) | (operand & (u8)r_SRS::N) | (u8)r_SRS::S;
}

u64 sfot::EmulateCycles(sfotmem& _memory, u64& _cycleAmount)
{
	while (e_Cycles < _cycleAmount)
	{
		EmulateStep(_memory);
	}
	return (e_Cycles - _cycleAmount);
}

void sfot::EmulateStep(sfotmem& _memory)
{
	// Get opcode from memory block
	u8 opcode = _memory[r_PC]; ++r_PC;

	u16 addr = 0;
	switch (e_OCAM[opcode])
	{
	case (u8)r_AM::IMM: { addr = r_PC; ++r_PC; break; } // Immediate addressing
	case (u8)r_AM::NINST: { return;  break; } // No instruction to execute (NOP & out of mem)
	case (u8)r_AM::NOADDR: { break; } // No address needed 
	default:
		// Get the address from the opcodes addressing mode
		addr = (this->*e_AMJT[e_OCAM[opcode]])(_memory);
	}

	// Execute the instruction
	(this->*e_OCJT[opcode])(addr, _memory);
}

sfot::sfot()
{
	// Set OCAM & OCJT to NoInsts
	for (u32 i = 0; i < 256; ++i) { e_OCAM[i] = (u8)r_AM::NINST; e_OCJT[i] = 0; }

	// Set up adressing jump-table
	e_AMJT[0] = &sfot::AM_Abs;
	e_AMJT[1] = &sfot::AM_AbsX;
	e_AMJT[2] = &sfot::AM_AbsY;
	e_AMJT[3] = &sfot::AM_Ind;
	e_AMJT[4] = &sfot::AM_XInd;
	e_AMJT[5] = &sfot::AM_IndY;
	e_AMJT[6] = &sfot::AM_Rel;
	e_AMJT[7] = &sfot::AM_Zpg;
	e_AMJT[8] = &sfot::AM_ZpgX;
	e_AMJT[9] = &sfot::AM_ZpgY;

	// Set up instruction jump-table & opcode addressing
		// Load/store operations
	// Load accumulator
	e_OCJT[(u8)sfotops::LDA_IMM] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_IMM] = (u8)r_AM::IMM;

	e_OCJT[(u8)sfotops::LDA_ZPG] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_ZPG] = (u8)r_AM::ZPG;

	e_OCJT[(u8)sfotops::LDA_ZPGX] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_ZPGX] = (u8)r_AM::ZPGX;

	e_OCJT[(u8)sfotops::LDA_ABS] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_ABS] = (u8)r_AM::ABS;

	e_OCJT[(u8)sfotops::LDA_ABSX] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_ABSX] = (u8)r_AM::ABSX;

	e_OCJT[(u8)sfotops::LDA_ABSY] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_ABSY] = (u8)r_AM::ABSY;

	e_OCJT[(u8)sfotops::LDA_XIND] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_XIND] = (u8)r_AM::XIND;

	e_OCJT[(u8)sfotops::LDA_INDY] = &sfot::O_LDA;
	e_OCAM[(u8)sfotops::LDA_INDY] = (u8)r_AM::INDY;

	// Load X register
	e_OCJT[(u8)sfotops::LDX_IMM] = &sfot::O_LDX;
	e_OCAM[(u8)sfotops::LDX_IMM] = (u8)r_AM::IMM;
						  
	e_OCJT[(u8)sfotops::LDX_ZPG] = &sfot::O_LDX;
	e_OCAM[(u8)sfotops::LDX_ZPG] = (u8)r_AM::ZPG;
						  
	e_OCJT[(u8)sfotops::LDX_ZPGY] = &sfot::O_LDX;
	e_OCAM[(u8)sfotops::LDX_ZPGY] = (u8)r_AM::ZPGY;
						  
	e_OCJT[(u8)sfotops::LDX_ABS] = &sfot::O_LDX;
	e_OCAM[(u8)sfotops::LDX_ABS] = (u8)r_AM::ABS;
						  
	e_OCJT[(u8)sfotops::LDX_ABSY] = &sfot::O_LDX;
	e_OCAM[(u8)sfotops::LDX_ABSY] = (u8)r_AM::ABSY;

	// Load Y register
	e_OCJT[(u8)sfotops::LDY_IMM] = &sfot::O_LDY;
	e_OCAM[(u8)sfotops::LDY_IMM] = (u8)r_AM::IMM;
						  
	e_OCJT[(u8)sfotops::LDY_ZPG] = &sfot::O_LDY;
	e_OCAM[(u8)sfotops::LDY_ZPG] = (u8)r_AM::ZPG;
						  
	e_OCJT[(u8)sfotops::LDY_ZPGX] = &sfot::O_LDY;
	e_OCAM[(u8)sfotops::LDY_ZPGX] = (u8)r_AM::ZPGX;
						  
	e_OCJT[(u8)sfotops::LDY_ABS] = &sfot::O_LDY;
	e_OCAM[(u8)sfotops::LDY_ABS] = (u8)r_AM::ABS;
						  
	e_OCJT[(u8)sfotops::LDY_ABSX] = &sfot::O_LDY;
	e_OCAM[(u8)sfotops::LDY_ABSX] = (u8)r_AM::ABSX;

	// Store accumulator to memory
	e_OCJT[(u8)sfotops::STA_ZPG] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::STA_ZPGX] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::STA_ABS] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::STA_ABSX] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::STA_ABSY] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::STA_XIND] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::STA_INDY] = &sfot::O_STA;
	e_OCAM[(u8)sfotops::STA_INDY] = (u8)r_AM::INDY;

	// Store X register to memory
	e_OCJT[(u8)sfotops::STX_ZPG] = &sfot::O_STX;
	e_OCAM[(u8)sfotops::STX_ZPG] = (u8)r_AM::ZPG;
						  
	e_OCJT[(u8)sfotops::STX_ZPGY] = &sfot::O_STX;
	e_OCAM[(u8)sfotops::STX_ZPGY] = (u8)r_AM::ZPGY;
						  
	e_OCJT[(u8)sfotops::STX_ABS] = &sfot::O_STX;
	e_OCAM[(u8)sfotops::STX_ABS] = (u8)r_AM::ABS;

	// Store Y register to memory
	e_OCJT[(u8)sfotops::STY_ZPG] = &sfot::O_STY;
	e_OCAM[(u8)sfotops::STY_ZPG] = (u8)r_AM::ZPG;
						  
	e_OCJT[(u8)sfotops::STY_ZPGX] = &sfot::O_STY;
	e_OCAM[(u8)sfotops::STY_ZPGX] = (u8)r_AM::ZPGX;
						  
	e_OCJT[(u8)sfotops::STY_ABS] = &sfot::O_STY;
	e_OCAM[(u8)sfotops::STY_ABS] = (u8)r_AM::ABS;

		// Register transfers
	// Copy accumulator to X register
	e_OCJT[(u8)sfotops::TAX] = &sfot::O_TAX;
	e_OCAM[(u8)sfotops::TAX] = (u8)r_AM::NOADDR;

	// Copy accumulator to Y register
	e_OCJT[(u8)sfotops::TAY] = &sfot::O_TAY;
	e_OCAM[(u8)sfotops::TAY] = (u8)r_AM::NOADDR;

	// Copy X register to accumulator
	e_OCJT[(u8)sfotops::TXA] = &sfot::O_TXA;
	e_OCAM[(u8)sfotops::TXA] = (u8)r_AM::NOADDR;

	// Copy Y register to accumulator
	e_OCJT[(u8)sfotops::TYA] = &sfot::O_TYA;
	e_OCAM[(u8)sfotops::TYA] = (u8)r_AM::NOADDR;

		// Stack operations
	// Copy stack pointer to X register
	e_OCJT[(u8)sfotops::TSX] = &sfot::O_TSX;
	e_OCAM[(u8)sfotops::TSX] = (u8)r_AM::NOADDR;

	// Copy X register to stack pointer
	e_OCJT[(u8)sfotops::TXS] = &sfot::O_TXS;
	e_OCAM[(u8)sfotops::TXS] = (u8)r_AM::NOADDR;

	// Push accumulator to stack
	e_OCJT[(u8)sfotops::PHA] = &sfot::O_PHA;
	e_OCAM[(u8)sfotops::PHA] = (u8)r_AM::NOADDR;

	// Push status register to stack
	e_OCJT[(u8)sfotops::PHP] = &sfot::O_PHP;
	e_OCAM[(u8)sfotops::PHP] = (u8)r_AM::NOADDR;

	// Pop to accumulator
	e_OCJT[(u8)sfotops::PLA] = &sfot::O_PLA;
	e_OCAM[(u8)sfotops::PLA] = (u8)r_AM::NOADDR;

	// Pop to status register
	e_OCJT[(u8)sfotops::PLP] = &sfot::O_PLP;
	e_OCAM[(u8)sfotops::PLP] = (u8)r_AM::NOADDR;

		// Logical
	// Logical AND with accumulator and memory
	e_OCJT[(u8)sfotops::AND_IMM] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::AND_ZPG] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::AND_ZPGX] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::AND_ABS] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::AND_ABSX] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::AND_ABSY] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::AND_XIND] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::AND_INDY] = &sfot::O_AND;
	e_OCAM[(u8)sfotops::AND_INDY] = (u8)r_AM::INDY;

	// Exclusive OR with accumulator and memory
	e_OCJT[(u8)sfotops::EOR_IMM] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::EOR_ZPG] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::EOR_ZPGX] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::EOR_ABS] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::EOR_ABSX] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::EOR_ABSY] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::EOR_XIND] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::EOR_INDY] = &sfot::O_EOR;
	e_OCAM[(u8)sfotops::EOR_INDY] = (u8)r_AM::INDY;

	// Inclusive OR with accumulator and memory
	e_OCJT[(u8)sfotops::ORA_IMM] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::ORA_ZPG] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::ORA_ZPGX] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::ORA_ABS] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::ORA_ABSX] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::ORA_ABSY] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::ORA_XIND] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::ORA_INDY] = &sfot::O_ORA;
	e_OCAM[(u8)sfotops::ORA_INDY] = (u8)r_AM::INDY;

	// Bit test with accumulator and memory
	e_OCJT[(u8)sfotops::BIT_ZPG] = &sfot::O_BIT;
	e_OCAM[(u8)sfotops::BIT_ZPG] = (u8)r_AM::ZPG;

	e_OCJT[(u8)sfotops::BIT_ABS] = &sfot::O_BIT;
	e_OCAM[(u8)sfotops::BIT_ABS] = (u8)r_AM::ABS;

		// Arithmetic
	// Add with carry to accumulator
	e_OCJT[(u8)sfotops::ADC_IMM] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::ADC_ZPG] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::ADC_ZPGX] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::ADC_ABS] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::ADC_ABSX] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::ADC_ABSY] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::ADC_XIND] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::ADC_INDY] = &sfot::O_ADC;
	e_OCAM[(u8)sfotops::ADC_INDY] = (u8)r_AM::INDY;

	// Subtract with carry to accumulator
	e_OCJT[(u8)sfotops::SBC_IMM] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::SBC_ZPG] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::SBC_ZPGX] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::SBC_ABS] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::SBC_ABSX] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::SBC_ABSY] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::SBC_XIND] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::SBC_INDY] = &sfot::O_SBC;
	e_OCAM[(u8)sfotops::SBC_INDY] = (u8)r_AM::INDY;

	// Compare accumulator to memory
	e_OCJT[(u8)sfotops::CMP_IMM] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_IMM] = (u8)r_AM::IMM;
						
	e_OCJT[(u8)sfotops::CMP_ZPG] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::CMP_ZPGX] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::CMP_ABS] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::CMP_ABSX] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_ABSX] = (u8)r_AM::ABSX;
						
	e_OCJT[(u8)sfotops::CMP_ABSY] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_ABSY] = (u8)r_AM::ABSY;
						
	e_OCJT[(u8)sfotops::CMP_XIND] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_XIND] = (u8)r_AM::XIND;
						
	e_OCJT[(u8)sfotops::CMP_INDY] = &sfot::O_CMP;
	e_OCAM[(u8)sfotops::CMP_INDY] = (u8)r_AM::INDY;

	// Compare X register to memory
	e_OCJT[(u8)sfotops::CPX_IMM] = &sfot::O_CPX;
	e_OCAM[(u8)sfotops::CPX_IMM] = (u8)r_AM::IMM;
						 
	e_OCJT[(u8)sfotops::CPX_ZPG] = &sfot::O_CPX;
	e_OCAM[(u8)sfotops::CPX_ZPG] = (u8)r_AM::ZPG;
						 
	e_OCJT[(u8)sfotops::CPX_ABS] = &sfot::O_CPX;
	e_OCAM[(u8)sfotops::CPX_ABS] = (u8)r_AM::ABS;

	// Compare Y register to memory
	e_OCJT[(u8)sfotops::CPY_IMM] = &sfot::O_CPY;
	e_OCAM[(u8)sfotops::CPY_IMM] = (u8)r_AM::IMM;
						  
	e_OCJT[(u8)sfotops::CPY_ZPG] = &sfot::O_CPY;
	e_OCAM[(u8)sfotops::CPY_ZPG] = (u8)r_AM::ZPG;
						  
	e_OCJT[(u8)sfotops::CPY_ABS] = &sfot::O_CPY;
	e_OCAM[(u8)sfotops::CPY_ABS] = (u8)r_AM::ABS;
	
		// Increments & decrements
	// Increment memory
	e_OCJT[(u8)sfotops::INC_ZPG] = &sfot::O_INC;
	e_OCAM[(u8)sfotops::INC_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::INC_ZPGX] = &sfot::O_INC;
	e_OCAM[(u8)sfotops::INC_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::INC_ABS] = &sfot::O_INC;
	e_OCAM[(u8)sfotops::INC_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::INC_ABSX] = &sfot::O_INC;
	e_OCAM[(u8)sfotops::INC_ABSX] = (u8)r_AM::ABSX;

	// Increment X register
	e_OCJT[(u8)sfotops::INX] = &sfot::O_INX;
	e_OCAM[(u8)sfotops::INX] = (u8)r_AM::NOADDR;

	// Increment Y register
	e_OCJT[(u8)sfotops::INY] = &sfot::O_INY;
	e_OCAM[(u8)sfotops::INY] = (u8)r_AM::NOADDR;

	// Decrement memory
	e_OCJT[(u8)sfotops::DEC_ZPG] = &sfot::O_DEC;
	e_OCAM[(u8)sfotops::DEC_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::DEC_ZPGX] = &sfot::O_DEC;
	e_OCAM[(u8)sfotops::DEC_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::DEC_ABS] = &sfot::O_DEC;
	e_OCAM[(u8)sfotops::DEC_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::DEC_ABSX] = &sfot::O_DEC;
	e_OCAM[(u8)sfotops::DEC_ABSX] = (u8)r_AM::ABSX;

	// Decrement X register
	e_OCJT[(u8)sfotops::DEX] = &sfot::O_DEX;
	e_OCAM[(u8)sfotops::DEX] = (u8)r_AM::NOADDR;

	// Decrement Y register
	e_OCJT[(u8)sfotops::DEY] = &sfot::O_DEY;
	e_OCAM[(u8)sfotops::DEY] = (u8)r_AM::NOADDR;

		// Shifts
	// Arithmetic shift left
	e_OCJT[(u8)sfotops::ASL_A] = &sfot::O_ASL_A;
	e_OCAM[(u8)sfotops::ASL_A] = (u8)r_AM::NOADDR;

	e_OCJT[(u8)sfotops::ASL_ZPG] = &sfot::O_ASL;
	e_OCAM[(u8)sfotops::ASL_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::ASL_ZPGX] = &sfot::O_ASL;
	e_OCAM[(u8)sfotops::ASL_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::ASL_ABS] = &sfot::O_ASL;
	e_OCAM[(u8)sfotops::ASL_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::ASL_ABSX] = &sfot::O_ASL;
	e_OCAM[(u8)sfotops::ASL_ABSX] = (u8)r_AM::ABSX;

	// Logical shift right
	e_OCJT[(u8)sfotops::LSR_A] = &sfot::O_LSR_A;
	e_OCAM[(u8)sfotops::LSR_A] = (u8)r_AM::NOADDR;
						
	e_OCJT[(u8)sfotops::LSR_ZPG] = &sfot::O_LSR;
	e_OCAM[(u8)sfotops::LSR_ZPG] = (u8)r_AM::ZPG;
						
	e_OCJT[(u8)sfotops::LSR_ZPGX] = &sfot::O_LSR;
	e_OCAM[(u8)sfotops::LSR_ZPGX] = (u8)r_AM::ZPGX;
						
	e_OCJT[(u8)sfotops::LSR_ABS] = &sfot::O_LSR;
	e_OCAM[(u8)sfotops::LSR_ABS] = (u8)r_AM::ABS;
						
	e_OCJT[(u8)sfotops::LSR_ABSX] = &sfot::O_LSR;
	e_OCAM[(u8)sfotops::LSR_ABSX] = (u8)r_AM::ABSX;

		// Jumps & calls

		// Branches

		// Status flag changes

		// System functions
	e_OCJT[(u8)sfotops::NOP] = 0;
	e_OCAM[(u8)sfotops::NOP] = (u8)r_AM::NINST;
}