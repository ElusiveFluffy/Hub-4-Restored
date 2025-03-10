#pragma once
#include "UIStructs.h"

//TygerMemory
#include "core.h"

namespace LevelSelect
{
	void SetupMultiColumn();
	void HandleInput();
	void MoveSelectionHorizontally(bool pressedRight);

	inline UIButtonGroup* GetUIButtonGroup() { return (UIButtonGroup*)(Core::moduleBase + 0x286cd0); };
};

