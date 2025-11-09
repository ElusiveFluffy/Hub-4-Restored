#include "pch.h"
#include "Levels.h"
#include "TyFunctions.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"
#include "ConditionalScript.h"
#include "SaveFile.h"

//TygerMemory
#include "core.h"
#include "hero.h"
#include "level.h"

static void ChangeBullLevel() {
	LevelCode level = Level::getCurrentLevel();
	//Allows bull to spawn if either outback or min min
	if (level == LevelCode::B3 || level == LevelCode::D1)
		Hero::SetBullHardCodeLevel(level);
	TyFunctions::Original_OneOfTheLoadFunctions();
}

using CheckScriptCondition_t = bool(__thiscall*)(ConditionalScript* script);
CheckScriptCondition_t Original_CheckScriptCondition;

static bool __fastcall CheckScriptCondition(ConditionalScript* script)
{
	Hub4SaveDataStruct* saveData = SaveFile::GetHub4SaveData();
	bool conditionFulfilled = false;
	switch (script->ConditionID) {
	// In Ty's code it actually checks D4, due to crikey being the boss of the B zone
	case BeenB:
		conditionFulfilled = saveData->LevelData[(int)LevelCode::B4].TimesEntered != 0;
		break;
	case BeenD:
		conditionFulfilled = saveData->LevelData[(int)LevelCode::D4].TimesEntered != 0;
		break;
	// Every other condition just go through the original function
	default:
		return Original_CheckScriptCondition(script);
	}

	// Flips the value if negative is true
	return conditionFulfilled != script->Negative;
}

void Levels::Init()
{
	HookFunctions();
	EditFMVLevels();
}

void Levels::HookFunctions()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x14ee60), &ChangeBullLevel, reinterpret_cast<LPVOID*>(&TyFunctions::Original_OneOfTheLoadFunctions));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create One of the Load Funtion Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x14d200), &CheckScriptCondition, reinterpret_cast<LPVOID*>(&Original_CheckScriptCondition));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Check Script Condition Hook, With the Error: " + error, Error);
		return;
	}
}

void Levels::EditFMVLevels()
{
	char* jumpTableIndices = (char*)(Core::moduleBase + 0x10c4b4);

	// Change B4 to jump to the platypus talisman (talisman 2) FMV, and make D4 jump out of the switch
	char newJumpTableIndex = 4;
	Core::SetReadOnlyValue(&jumpTableIndices[(int)LevelCode::B4], &newJumpTableIndex, 1);

	newJumpTableIndex = 5;
	Core::SetReadOnlyValue(&jumpTableIndices[(int)LevelCode::D4], &newJumpTableIndex, 1);
}
