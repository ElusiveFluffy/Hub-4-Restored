#include "pch.h"
#include "Smasharang.h"
#include "MinHook.h"
#include "Rangs.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"

typedef int(__thiscall* ActorHit_t)(void* actor, int* eventID);
ActorHit_t Original_SpyEggHit;
ActorHit_t Original_E4MetalDoorEvent;

//Random kinda pointless thing just to have it match Ty 2 xD
int __fastcall SpyEggHit(void* spyEggActor, void* edx, int* eventID) {
	//Only change it to a event id of 9 to break it if its the smasharang
	//For rangs the event id will always be set to 8
	if (*eventID == 8 && Rangs::GetCurrentRang() == Rangs::Smasharang)
		*eventID = 9;
	return Original_SpyEggHit(spyEggActor, eventID);
}

int __fastcall E4MetalDoorEvent(void* metalDoorActor, void* edx, int* eventID) {
	if (*eventID == 8 && Rangs::GetCurrentRang() == Rangs::Smasharang || Rangs::GetCurrentRang() == Rangs::Kaboomerang)
		return Original_E4MetalDoorEvent(metalDoorActor, eventID);
	//Don't run the rang event if it isn't the smasharang or kaboomerang
	if (*eventID != 8)
		return Original_E4MetalDoorEvent(metalDoorActor, eventID);
}

void Smasharang::HookSmasharangDamages()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x5f140), &SpyEggHit, reinterpret_cast<LPVOID*>(&Original_SpyEggHit));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Spy Egg Damage Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x128410), &E4MetalDoorEvent, reinterpret_cast<LPVOID*>(&Original_E4MetalDoorEvent));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the E4 Metal Door Event Function Hook, With the Error: " + error, Error);
		return;
	}
}
