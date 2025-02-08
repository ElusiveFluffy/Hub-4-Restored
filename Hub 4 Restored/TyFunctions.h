#pragma once
#include "sharedEnums.h"

namespace TyFunctions {
	typedef int(__thiscall* TyCountOpals_t)(void* properties, int levelIndex);
	int CountOpals(LevelCode levelIndex);

	typedef int(*SetUpFullGameTotals_t)();
	inline SetUpFullGameTotals_t Original_SetUpFullGameTotals;

	typedef void(*VoidFunction_t)();
	inline VoidFunction_t Original_OneOfTheLoadFunctions;

	void SetFuntions();
}
