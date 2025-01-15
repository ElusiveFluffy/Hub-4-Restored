#pragma once
#include "core.h"
#include "level.h"

namespace TyMemoryValues {

	//To disable the game info option in the credits
	void inline DisableGameInfoID(LevelCode levelID) { 
		Core::SetReadOnlyValue((BYTE*)(Core::moduleBase + 0xE42E2), &levelID, 1);
	};
}