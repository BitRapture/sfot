#ifndef _SFOTMEM_H_
#define _SFOTMEM_H_

#include "defs.h"

// A block of memory for the sfot
struct sfotmem
{
public: // Memory 
	// Dynamic memory block
	u8* m_Memory{ nullptr };
	// Amount of memory (0 == 1) (u16 + 1)
	u32 m_Size{ 0 };

public: // Operator overloading
	u8 operator[](const u16& _index) { return (_index < m_Size ? m_Memory[_index] : (u8)sfotops::NOP); }
	void Set(const u16& _index, const u8& _e) { (_index < m_Size ? m_Memory[_index] = _e : 0); }

public: // Instantiation
	/// @brief Initialize sfotmem
	/// @param _size Amount of bytes to allocate
	sfotmem(u16 _size = 65535) : m_Size{ _size } { ++m_Size; m_Memory = new u8[m_Size]{ 0 }; };
	/// @brief Initialize sfotmem with external memory
	/// @param _memory The external memory
	/// @param _size Size of external memory
	sfotmem(u8* _memory, u16 _size) : m_Size{ _size }, m_Memory{ _memory } { ++m_Size; };
	// Destruction
	~sfotmem() { delete[] m_Memory; };
};

static inline void DeleteSFOTMem(sfotmem& _memory) { delete[] _memory.m_Memory; _memory.m_Memory = nullptr; _memory.m_Size = 0; }

static inline void ConcatSFOTMem(u32 _index, sfotmem& _memory, u8* _mem, u32 _memSize) { for (u32 i = 0; i < _memSize; ++i) { if (_index + i >= _memory.m_Size) { break; } _memory.m_Memory[_index + i] = _mem[i]; } }

static inline void FillOPSFOTMem(sfotmem& _memory, u8 _opCode) { for (u32 i = 0; i < _memory.m_Size; ++i) { _memory.m_Memory[i] = _opCode; }; }

#endif // !_SFOTMEM_H_
