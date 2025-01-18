#include "pch.h"
#include "Collectables.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"
#include "TyFunctions.h"

//TygerMemory
#include "collectable.h"
#include "level.h"
#include "core.h"
#include "totals.h"
#include "savedata.h"

void SetEarthThunderEggParticle() {
	Collectable::GetThunderEggParticleColours()[Element::Earth] = FloatRGBA{ 0.8f, 0.8f, 0.0f, 1.0f };
	API::LogPluginMessage("Set Earth Thunder Egg Particle Colours");
}

void SetLevelInfo() {
	LevelInfo* levelInfo = Collectable::GetLevelInfoArray();
	levelInfo[(int)LevelCode::Z2].ShowOpal = false;

	levelInfo[(int)LevelCode::C1].Element = Earth;
	levelInfo[(int)LevelCode::C2].Element = Earth;
	levelInfo[(int)LevelCode::C3].Element = Earth;
	levelInfo[(int)LevelCode::C4].Element = Earth;

	levelInfo[(int)LevelCode::D1].Zone = Zone::DZone;
	levelInfo[(int)LevelCode::D1].ShowOpal = true;
	levelInfo[(int)LevelCode::D1].ShowExtendedTotals = true;

	levelInfo[(int)LevelCode::D2].Zone = Zone::DZone;
	levelInfo[(int)LevelCode::D2].ShowOpal = true;
	levelInfo[(int)LevelCode::D2].ShowExtendedTotals = true;

	levelInfo[(int)LevelCode::D3].Zone = Zone::DZone;
	levelInfo[(int)LevelCode::D3].ShowOpal = true;
	levelInfo[(int)LevelCode::D3].ShowExtendedTotals = true;

	API::LogPluginMessage("Set Level Info Properties");
}

void SetFullGameTotals() {
	Totals::SetTotalOpals(3000);
	Totals::SetTotalCogs(100);
	Totals::SetTotalBilbies(50);
	Totals::SetTotalThunderEggs(80);

	API::LogPluginMessage("Set Full Game Totals");
}

int TotalsCountDLevels() {
	// Run the original function that sets up the full game totals, 
	// so that I don't need to redo counting the levels that already have their opals counted
	int result = TyFunctions::Original_SetUpFullGameTotals();

	SaveDataStruct* saveData = SaveData::GetData();
	int* TotalCollectedOpals = (int*)(Core::moduleBase + 0x261878);

	// Checks if you've collected the 300 opal thunder egg (since the opals bits get reset after doing that)
	if (!saveData->LevelData[(int)LevelCode::D1].ThunderEggs[0])
		*TotalCollectedOpals += TyFunctions::CountOpals(LevelCode::D1);
	else
		*TotalCollectedOpals += 300;
	// Checks if you've collected the 300 opal thunder egg (since the opals bits get reset after doing that)
	if (!saveData->LevelData[(int)LevelCode::D2].ThunderEggs[0])
		*TotalCollectedOpals += TyFunctions::CountOpals(LevelCode::D2);
	else
		*TotalCollectedOpals += 300;
	// Checks if you've collected the 300 opal thunder egg (since the opals bits get reset after doing that)
	if (!saveData->LevelData[(int)LevelCode::D3].ThunderEggs[0])
		*TotalCollectedOpals += TyFunctions::CountOpals(LevelCode::D3);
	else
		*TotalCollectedOpals += 300;

	return result;
}

int __fastcall CalculateCompletion(void* contextMaybe) {

	int collectableCount = 0;

	collectableCount += Totals::getCurrentTalismanCount();
	collectableCount += Totals::getCurrentCogCount(Global);
	collectableCount += Totals::getCurrentThunderEggCount(Global);

	int completionPercent = (collectableCount * 100) / 185;

	//Same fail safe as the game has
	if (completionPercent > 100)
		completionPercent = 100;

	return completionPercent;
}

void HookGameTotalsFunctions() {
	//Hook Ty Shutdown Function
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xE7530), &TotalsCountDLevels, reinterpret_cast<LPVOID*>(&TyFunctions::Original_SetUpFullGameTotals));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Game Totals Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xf7200), &CalculateCompletion, reinterpret_cast<LPVOID*>(&TyFunctions::Original_CalculateCompletion));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Calculate Completion Function Hook, With the Error: " + error, Error);
		return;
	}

	//Enable both hooks
	minHookStatus = MH_EnableHook(MH_ALL_HOOKS);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Hook Game Totals Function, With the Error: " + error, Error);
		return;
	}

	API::LogPluginMessage("Sucessfully Hooked the Game Totals Function");
}

void Collectables::Setup()
{
	SetEarthThunderEggParticle();
	SetLevelInfo();
	SetFullGameTotals();
	Totals::SetGameInfoLevels(GameInfoLevelIndexes, std::size(GameInfoLevelIndexes));
	HookGameTotalsFunctions();
}
