#include "pch.h"
#include "WaterTank.h"
#include "MinHook.h"
#include "GameObject.h"

//TygerMemory
#include "core.h"
#include "level.h"

GameObjectVoid_t OriginalWaterTankLoadDone;

void __fastcall WaterTankLoadDone(GameObject* prop) {
	OriginalWaterTankLoadDone(prop);

	if (Level::getCurrentLevel() == LevelCode::D1)
		prop->pModel->Colour = D1PropColour;
}

void WaterTank::HookFunctions()
{
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x1788f0), &WaterTankLoadDone, reinterpret_cast<LPVOID*>(&OriginalWaterTankLoadDone));
}
