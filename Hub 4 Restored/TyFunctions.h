#pragma once
#include "sharedEnums.h"

namespace TyFunctions {
	typedef int(__thiscall* TyCountOpals_t)(void* properties, int levelIndex);
	int CountOpals(LevelCode levelIndex);

	typedef int(*SetUpFullGameTotals_t)();
	inline SetUpFullGameTotals_t Original_SetUpFullGameTotals;

	typedef int(__fastcall* CalculateCompletion_t)();
	inline CalculateCompletion_t Original_CalculateCompletion;

	void SetFuntions();
}
