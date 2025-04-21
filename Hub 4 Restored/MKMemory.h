#pragma once
//TygerMemory
#include "core.h"

namespace MKMemory {
	typedef void* (*Heap_MemAlloc_t) (int size);
	inline Heap_MemAlloc_t Heap_MemAlloc;

	inline void SetFunctions() {
		Heap_MemAlloc = (Heap_MemAlloc_t)(Core::moduleBase + 0x196840);
	}
}