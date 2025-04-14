#include "pch.h"
#include "Smasharang.h"
#include "MinHook.h"
#include "Rangs.h"
#include "GameObject.h"
#include "MKMessage.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"

GameObject::GameObjectMsg_t Original_SpyEggHit;
GameObject::GameObjectMsg_t Original_E4MetalDoorEvent;

//Random kinda pointless thing just to have it match Ty 2 xD
void __fastcall SpyEggHit(GameObject::MKProp* spyEggActor, void* edx, MKMessage* msg) {
	//Only change it to a event id of 9 to break it if its the smasharang
	//For rangs the event id will always be set to 8
	if (msg->MsgID == BoomerangMsg && Rangs::GetCurrentRang() == Rangs::Smasharang)
		msg->MsgID = ExplosionMsg;
	Original_SpyEggHit(spyEggActor, msg);
}

void __fastcall E4MetalDoorEvent(GameObject::MKProp* metalDoorActor, void* edx, MKMessage* msg) {
	if (msg->MsgID == ExplosionMsg || (msg->MsgID == BoomerangMsg && (Rangs::GetCurrentRang() == Rangs::Smasharang || Rangs::GetCurrentRang() == Rangs::Kaboomerang)))
		Original_E4MetalDoorEvent(metalDoorActor, msg);
	//Don't run the rang event if it isn't the smasharang or kaboomerang
	else if (msg->MsgID != BoomerangMsg)
		Original_E4MetalDoorEvent(metalDoorActor, msg);
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
