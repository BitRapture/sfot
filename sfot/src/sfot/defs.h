#ifndef _DEFS_H_
#define _DEFS_H_

/// Definitions for sfot

// 8 bits
using u8 = unsigned char;
// 8 bits signed
using s8 = char;
// 16 bits
using u16 = unsigned short;
// 32 bits
using u32 = unsigned int;
// 64 bits
using u64 = unsigned long int;

// Common sfot memory sizes (kibibytes)
enum class sfotcmem { M1K = 1024, M8K = 8192, M16K = 16384, M32K = 32768, M64K = 65535 };

// Opcodes names
enum class sfotops {
	LDA_IMM = 0xA9, LDA_ZPG = 0xA5, LDA_ZPGX = 0xB5, LDA_ABS = 0xAD, LDA_ABSX = 0xBD, LDA_ABSY = 0xB9, LDA_XIND = 0xA1, LDA_INDY = 0xB1,
	LDX_IMM = 0xA2, LDX_ZPG = 0xA6, LDX_ZPGY = 0xB6, LDX_ABS = 0xAE, LDX_ABSY = 0xBE,
	LDY_IMM = 0xA0, LDY_ZPG = 0xA4, LDY_ZPGX = 0xB4, LDY_ABS = 0xAC, LDY_ABSX = 0xBC,
	STA_ZPG = 0x85, STA_ZPGX = 0x95, STA_ABS = 0x8D, STA_ABSX = 0x9D, STA_ABSY = 0x99, STA_XIND = 0x81, STA_INDY = 0x91,
	STX_ZPG = 0x86, STX_ZPGY = 0x96, STX_ABS = 0x8E,
	STY_ZPG = 0x84, STY_ZPGX = 0x94, STY_ABS = 0x8C,
	TAX = 0xAA, TAY = 0xA8, TXA = 0x8A, TYA = 0x98,
	TSX = 0xBA, TXS = 0x9A, PHA = 0x48, PHP = 0x08, PLA = 0x68, PLP = 0x28,
	AND_IMM = 0x29, AND_ZPG = 0x25, AND_ZPGX = 0x35, AND_ABS = 0x2D, AND_ABSX = 0x3D, AND_ABSY = 0x39, AND_XIND = 0x21, AND_INDY = 0x31,
	EOR_IMM = 0x49, EOR_ZPG = 0x45, EOR_ZPGX = 0x55, EOR_ABS = 0x4D, EOR_ABSX = 0x5D, EOR_ABSY = 0x59, EOR_XIND = 0x41, EOR_INDY = 0x51,
	ORA_IMM = 0x09, ORA_ZPG = 0x05, ORA_ZPGX = 0x15, ORA_ABS = 0x0D, ORA_ABSX = 0x1D, ORA_ABSY = 0x19, ORA_XIND = 0x01, ORA_INDY = 0x11,
	BIT_ZPG = 0x24, BIT_ABS = 0x2C,
	ADC_IMM = 0x69, ADC_ZPG = 0x65, ADC_ZPGX = 0x75, ADC_ABS = 0x6D, ADC_ABSX = 0x7D, ADC_ABSY = 0x79, ADC_XIND = 0x61, ADC_INDY = 0x71,
	SBC_IMM = 0xE9, SBC_ZPG = 0xE5, SBC_ZPGX = 0xF5, SBC_ABS = 0xED, SBC_ABSX = 0xFD, SBC_ABSY = 0xF9, SBC_XIND = 0xE1, SBC_INDY = 0xF1,
	CMP_IMM = 0xC9, CMP_ZPG = 0xC5, CMP_ZPGX = 0xD5, CMP_ABS = 0xCD, CMP_ABSX = 0xDD, CMP_ABSY = 0xD9, CMP_XIND = 0xC1, CMP_INDY = 0xD1,
	CPX_IMM = 0xE0, CPX_ZPG = 0xE4, CPX_ABS = 0xEC,
	CPY_IMM = 0xC0, CPY_ZPG = 0xC4, CPY_ABS = 0xCC,
	INC_ZPG = 0xE6, INC_ZPGX = 0xF6, INC_ABS = 0xEE, INC_ABSX = 0xFE,
	INX = 0xE8, INY = 0xC8,
	DEC_ZPG = 0xC6, DEC_ZPGX = 0xD6, DEC_ABS = 0xCE, DEC_ABSX = 0xDE,
	DEX = 0xCA, DEY = 0x88,
	ASL_A = 0x0A, ASL_ZPG = 0x06, ASL_ZPGX = 0x16, ASL_ABS = 0x0E, ASL_ABSX = 0x1E,
	LSR_A = 0x4A, LSR_ZPG = 0x46, LSR_ZPGX = 0x56, LSR_ABS = 0x4E, LSR_ABSX = 0x5E,
	ROL_A = 0x2A, ROL_ZPG = 0x26, ROL_ZPGX = 0x36, ROL_ABS = 0x2E, ROL_ABSX = 0x3E,
	ROR_A = 0x6A, ROR_ZPG = 0x66, ROR_ZPGX = 0x76, ROR_ABS = 0x6E, ROR_ABSX = 0x7E,
	JMP_ABS = 0x4C, JMP_IND = 0x6c,
	JSR_ABS = 0x20,
	RTS = 0x60,
	BCC = 0x90, BCS = 0xB0, BEQ = 0xF0, BMI = 0x30, BNE = 0xD0, BPL = 0x10, BVC = 0x50, BVS = 0x70,
	CLC = 0x18, CLD = 0xD8, CLI = 0x58, CLV = 0xB8, SEC = 0x38, SED = 0xF8, SEI = 0x78,
	BRK = 0x00,
	NOP = 0xEA,
	RTI = 0x40
};

#endif // !_DEFS_H_
