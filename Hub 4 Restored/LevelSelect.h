#pragma once
//TygerMemory
#include "core.h"

namespace LevelSelect
{
	void SetupMultiColumn();
	void HandleInput();
	void MoveSelectionHorizontally(bool pressedRight);

	inline int GetButtonInfoStart() { return *(int*)(Core::moduleBase + 0x286cd8); };
	inline int* GetSelectedLevelPtr() { return (int*)(Core::moduleBase + 0x286CD4); };
	inline int GetTextDeselectedColour() { return *(int*)(Core::moduleBase + 0x264164); };
	inline int GetTextSelectedColour() { return *(int*)(Core::moduleBase + 0x264168); };
};

