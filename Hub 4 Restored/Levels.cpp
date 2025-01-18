#include "pch.h"
#include "Levels.h"
#include "TyFunctions.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"
#include "hero.h"
#include "level.h"

void ChangeBullLevel() {
	LevelCode level = Level::getCurrentLevel();
	//Allows bull to spawn if either outback or min min
	if (level == LevelCode::B3 || level == LevelCode::D1)
		Hero::SetBullHardCodeLevel(level);
	TyFunctions::Original_OneOfTheLoadFunctions();
}

void Levels::HookFunctions()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x14ee60), &ChangeBullLevel, reinterpret_cast<LPVOID*>(&TyFunctions::Original_OneOfTheLoadFunctions));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create One of the Load Funtion Hook, With the Error: " + error, Error);
		return;
	}

	//Enable the hooks
	minHookStatus = MH_EnableHook(MH_ALL_HOOKS);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to One of the Load Functions, With the Error: " + error, Error);
		return;
	}
}
