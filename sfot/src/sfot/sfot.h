#ifndef _SFOT_H_
#define _SFOT_H_

#include "defs.h"
#include "sfotmem.h"

/// @brief An Emulated 6502 Processor
class sfot
{
private: // Defs
	// Status register statuses for ANDing
	// Carry, Zero, Interrupt, Decimal, Break, Constant, Overflow, Negative
	enum struct r_SRS { C=1, Z=2, I=4, D=8, B=16, S=32, O=64, N=128 };
	// Status register statuses for shifting
	// C Z I D B - O N
	enum struct r_SRSs { C=0, Z=1, I=2, D=3, B=4, S=5, O=6, N=7 };

	// Addressing modes
	enum struct r_AM { ABS, ABSX, ABSY, IND, XIND, INDY, REL, ZPG, ZPGX, ZPGY, IMM, NOADDR, NINST };

private: // Registers
	// Status register
	u8 r_SR{ 0b00100000 };
	// X and Y registers
	u8 r_X{ 0x0 }, r_Y{ 0x0 };
	// Stack pointer (descending stack)
	u8 r_S{ 0xff };
	// Accumulator register
	u8 r_A{ 0x0 };
	// Program counter
	u16 r_PC{ 0x0 };

private: // Emulation Variables
	// Cycle count
	u64 e_Cycles{ 0 };

	// Opcode instruction jump-table
	void (sfot::*e_OCJT[256])(u16&, sfotmem&);
	// Opcode addressing modes jump-table
	u8 e_OCAM[256];
	// Addressing mode instruction jump-table
	u16 (sfot::* e_AMJT[10])(sfotmem&);

public: // Configurable cpu vectors
	// BRK vector (High byte)
	u16 e_BRK_L{ 0xFFFE };
	// BRK vector (Low byte)
	u16 e_BRK_H{ 0xFFFF };


private: // Addressing Modes
	// Absolute address
	u16 AM_Abs(sfotmem& _mem);
	// Absolute X 
	u16 AM_AbsX(sfotmem& _mem);
	// Absolute Y
	u16 AM_AbsY(sfotmem& _mem);
	// Indirect address
	u16 AM_Ind(sfotmem& _mem);
	// X Indirect
	u16 AM_XInd(sfotmem& _mem);
	// Indirect Y
	u16 AM_IndY(sfotmem& _mem);
	// Relative address
	u16 AM_Rel(sfotmem& _mem);
	// Zero page address
	u16 AM_Zpg(sfotmem& _mem);
	// Zero page X
	u16 AM_ZpgX(sfotmem& _mem);
	// Zero page Y
	u16 AM_ZpgY(sfotmem& _mem);

private: // Opcodes/Instructions
	// Load/store operations
	void O_LDA(u16& _addr, sfotmem& _mem);
	void O_LDX(u16& _addr, sfotmem& _mem);
	void O_LDY(u16& _addr, sfotmem& _mem);
	void O_STA(u16& _addr, sfotmem& _mem);
	void O_STX(u16& _addr, sfotmem& _mem);
	void O_STY(u16& _addr, sfotmem& _mem);

	// Register transfers
	void O_TAX(u16& _addr, sfotmem& _mem);
	void O_TAY(u16& _addr, sfotmem& _mem);
	void O_TXA(u16& _addr, sfotmem& _mem);
	void O_TYA(u16& _addr, sfotmem& _mem);

	// Stack operations
	void O_TSX(u16& _addr, sfotmem& _mem);
	void O_TXS(u16& _addr, sfotmem& _mem);
	void O_PHA(u16& _addr, sfotmem& _mem);
	void O_PHP(u16& _addr, sfotmem& _mem);
	void O_PLA(u16& _addr, sfotmem& _mem);
	void O_PLP(u16& _addr, sfotmem& _mem);

	// Logical
	void O_AND(u16& _addr, sfotmem& _mem);
	void O_EOR(u16& _addr, sfotmem& _mem);
	void O_ORA(u16& _addr, sfotmem& _mem);
	void O_BIT(u16& _addr, sfotmem& _mem);

	// Arithmetic
	void O_ADC(u16& _addr, sfotmem& _mem);
	void O_SBC(u16& _addr, sfotmem& _mem);
	void O_CMP(u16& _addr, sfotmem& _mem);
	void O_CPX(u16& _addr, sfotmem& _mem);
	void O_CPY(u16& _addr, sfotmem& _mem);

	// Increments & decrements
	void O_INC(u16& _addr, sfotmem& _mem);
	void O_INX(u16& _addr, sfotmem& _mem);
	void O_INY(u16& _addr, sfotmem& _mem);
	void O_DEC(u16& _addr, sfotmem& _mem);
	void O_DEX(u16& _addr, sfotmem& _mem);
	void O_DEY(u16& _addr, sfotmem& _mem);

	// Shifts
	void O_ASL(u16& _addr, sfotmem& _mem); void O_ASL_A(u16& _addr, sfotmem& _mem);
	void O_LSR(u16& _addr, sfotmem& _mem); void O_LSR_A(u16& _addr, sfotmem& _mem);
	void O_ROL(u16& _addr, sfotmem& _mem); void O_ROL_A(u16& _addr, sfotmem& _mem);
	void O_ROR(u16& _addr, sfotmem& _mem); void O_ROR_A(u16& _addr, sfotmem& _mem);

	// Jumps & calls
	void O_JMP(u16& _addr, sfotmem& _mem);
	void O_JSR(u16& _addr, sfotmem& _mem);
	void O_RTS(u16& _addr, sfotmem& _mem);

	// Branches
	void O_BCC(u16& _addr, sfotmem& _mem);
	void O_BCS(u16& _addr, sfotmem& _mem);
	void O_BEQ(u16& _addr, sfotmem& _mem);
	void O_BMI(u16& _addr, sfotmem& _mem);
	void O_BNE(u16& _addr, sfotmem& _mem);
	void O_BPL(u16& _addr, sfotmem& _mem);
	void O_BVC(u16& _addr, sfotmem& _mem);
	void O_BVS(u16& _addr, sfotmem& _mem);

	// Status flag changes
	void O_CLC(u16& _addr, sfotmem& _mem);
	void O_CLD(u16& _addr, sfotmem& _mem);
	void O_CLI(u16& _addr, sfotmem& _mem);
	void O_CLV(u16& _addr, sfotmem& _mem);
	void O_SEC(u16& _addr, sfotmem& _mem);
	void O_SED(u16& _addr, sfotmem& _mem);
	void O_SEI(u16& _addr, sfotmem& _mem);
	
	// System functions
	void O_BRK(u16& _addr, sfotmem& _mem);
	void O_RTI(u16& _addr, sfotmem& _mem);

public: // Getters/Setters
	// Get status register
	const u8* GetStatusRegister() { return &r_SR; };
	// Get x register
	const u8* GetXRegister() { return &r_X; };
	// Get y register
	const u8* GetYRegister() { return &r_Y; };
	// Get accumulator register
	const u8* GetARegister() { return &r_A; };
	// Get stack pointer
	const u8* GetStackPointer() { return &r_S; };
	// Get program counter
	const u16* GetProgramCounter() { return &r_PC; };
	// Set program counter
	void SetProgramCounter(u16 _address) { r_PC = _address; };

	// Get cycle count
	const u64* GetCycleCount() { return &e_Cycles; };

public: // Emulation
	/// @brief Emulate the sfot up to a certain amount of cycles
	/// @param _memory The memory for the sfot
	/// @param _cycleAmount The amount of cycles before the processor will stop running
	/// @return The amount of cycles the sfot went over by 
	u64 EmulateCycles(sfotmem& _memory, u64& _cycleAmount);

	/// @brief Emulate the sfot for one step
	/// @param _memory The memory for the sfot
	void EmulateStep(sfotmem& _memory);

public: // Instantiation
	// Default construction
	sfot();
};

#endif // !_SFOT_H_
