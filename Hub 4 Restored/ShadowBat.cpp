#include "pch.h"
#include "ShadowBat.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"
#include "level.h"

PipeGetFallingPos_t Original_PipeGetFallingPos;
ShadowReset_t Original_Reset;

Vector4f* __fastcall PipeGetFallingPos(void* pPipe, void* edx, Vector4f* fallPos) {
	if (Level::getCurrentLevel() == LevelCode::B4)
	{
		*fallPos = { 0, 0, 0, 1 };
		return fallPos;
	}
	else
		return Original_PipeGetFallingPos(pPipe, fallPos);
}

void __fastcall Reset(ShadowBatProp* shadow) {
	int hp = 3;
	//For the 4 fireworks in the boss fight
	if (Level::getCurrentLevel() == LevelCode::B4)
		hp = 4;

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xbf5a8), &hp, 4);
	Original_Reset(shadow);
}

void HookFunction() {
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x13f070), &PipeGetFallingPos, reinterpret_cast<LPVOID*>(&Original_PipeGetFallingPos));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Pipe Get Falling Pos Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf500), &Reset, reinterpret_cast<LPVOID*>(&Original_Reset));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Reset Function Hook, With the Error: " + error, Error);
		return;
	}
}

void ShadowBat::InitFunctions()
{
	HookFunction();

	// Gives the B zone talisman instead of the D zone one (no need to change it based on level, since the D zone talisman would be gotten from crikey now)
	char newZoneComplete = (char)Zone::BZone;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc1083), &newZoneComplete, 1);
}

void ShadowBatProp::Damage()
{
	//Damages shadow
	StateManager.NextState = ShadowBat::Fall;
}

ShadowBatProp* ShadowBatProp::GetShadowBat()
{
	return *(ShadowBatProp**)(Core::moduleBase + 0x25da18);
}
