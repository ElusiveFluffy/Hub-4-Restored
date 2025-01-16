#include "pch.h"
#include "TyFunctions.h"
#include "core.h"

TyFunctions::TyCountOpals_t TyCountOpals;

int TyFunctions::CountOpals(LevelCode levelIndex)
{
	return TyCountOpals((void*)(Core::moduleBase + 0x2888ac), (int)levelIndex);
}

void TyFunctions::SetFuntions()
{
	TyCountOpals = (TyCountOpals_t)(Core::moduleBase + 0xf7060);
}
