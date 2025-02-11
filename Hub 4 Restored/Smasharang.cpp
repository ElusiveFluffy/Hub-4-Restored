#include "pch.h"
#include "Smasharang.h"
#include "MinHook.h"
#include "Rangs.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"

typedef int(__thiscall* SpyEggHit_t)(void* spyEggActor, int* damageType);
SpyEggHit_t Original_SpyEggHit;

//Random kinda pointless thing just to have it match Ty 2 xD
int __fastcall SpyEggHit(void* spyEggActor, void* edx, int* damageType) {
	//Only change it to a damage type of 9 to break it if its the smasharang
	//Normal rangs the damage type will always be set to 8 too
	if (*damageType == 8 && Rangs::GetCurrentRang() == Rangs::Smasharang)
		*damageType = 9;
	return Original_SpyEggHit(spyEggActor, damageType);
}

void Smasharang::HookSmasharangDamages()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x5f140), &SpyEggHit, reinterpret_cast<LPVOID*>(&Original_SpyEggHit));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create The Spy Egg Damage Function Hook, With the Error: " + error, Error);
		return;
	}
}
