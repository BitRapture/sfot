#include "sfot/sfot.h"

#include <iostream>

#define op (u8)sfotops::
#define cmem (u16)sfotcmem::

#define STEPS 1

int main()
{
	// Create 6502 system
	sfot processor;

	// Load some machine code
	u8* mem = new u8[65536]{
		op BRK,
		op LDX_IMM,
		0x80
	};

	mem[processor.e_BRK_L] = 0x00;
	mem[processor.e_BRK_H] = 0x02;

	mem[0x0200] = op ADC_IMM;
	mem[0x0201] = 0x01;
	mem[0x0202] = op RTI;

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
		<< "\nconstant: " << (u32)((*statusRegister & 32) >> 5)
		<< "\noverflow: " << (u32)((*statusRegister & 64) >> 6)
		<< "\nnegative: " << (u32)((*statusRegister & 128) >> 7)
		<< std::endl;

	return 0;
}