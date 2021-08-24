#include "sfot/sfot.h"

#include <iostream>

#define op (u8)sfotops::
#define cmem (u16)sfotcmem::
#define STEPS 100

int main()
{
	// Create 6502 system
	sfot processor;

	// Load some machine code
	u8* mem = new u8[65536]{
		op LDX_IMM,
		0x20,
		op INC_ABS,
		0x00,
		0x10,
		op DEX,
		op BNE,
		0xFB,
		op LDA_ABS,
		0x00,
		0x10
	};

	sfotmem memory(mem, cmem M64K);

	// Debug variables
	const u16* programCounter = processor.GetProgramCounter();
	const u8* accumulator = processor.GetARegister(),
		* xRegister = processor.GetXRegister(),
		* yRegister = processor.GetYRegister(),
		* stackPointer = processor.GetStackPointer(),
		* statusRegister = processor.GetStatusRegister();

	for (unsigned int i = 0; i < STEPS; ++i)
		processor.EmulateStep(memory);

	std::cout << "pc: " << *programCounter
		<< "\n\na-reg: " << (u32)*accumulator
		<< "\nx-reg: " << (u32)*xRegister
		<< "\ny-reg: " << (u32)*yRegister
		<< "\n\nstack-p: " << (u32)*stackPointer
		<< "\nstart of stack: " << (u32)mem[0x01ff]
		<< "\n\nstatus: " << (u32)*statusRegister 
		<< "\ncarry: " << (u32)(*statusRegister & 1)
		<< "\nzero: " << (u32)((*statusRegister & 2) >> 1)
		<< "\ninterrupt: " << (u32)((*statusRegister & 4) >> 2)
		<< "\ndecimal: " << (u32)((*statusRegister & 8) >> 3)
		<< "\nbreak: " << (u32)((*statusRegister & 16) >> 4)
		<< "\noverflow: " << (u32)((*statusRegister & 64) >> 6)
		<< "\nnegative: " << (u32)((*statusRegister & 128) >> 7)
		<< std::endl;

	return 0;
}