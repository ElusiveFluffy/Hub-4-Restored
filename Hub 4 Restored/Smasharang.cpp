#include "pch.h"
#include "Smasharang.h"
#include "MinHook.h"
#include "Rangs.h"
#include "GameObject.h"
#include "MKMessage.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"

GameObjectMsg_t Original_SpyEggHit;
GameObjectMsg_t Original_E4MetalDoorEvent;

//Random kinda pointless thing just to have it match Ty 2 xD
void __fastcall SpyEggMessage(GameObject* spyEggActor, void* edx, BoomerangMKMessage* msg) {
	//Only change it to a event id of 9 to break it if its the smasharang
	//For rangs the event id will always be set to 8
	if (msg->MsgID == MSG_BoomerangMsg && msg->HitRang->RangIndex == Rangs::Smasharang)
		msg->MsgID = MSG_ExplosionMsg;
	Original_SpyEggHit(spyEggActor, msg);
}

void Smasharang::SetSpyEggAutoTargetRang(int newRang)
{
	//The game already had this set to zappyrang, just adding the smasharang to it, since it can break spyeggs
	if (newRang == Rangs::Smasharang || newRang == Rangs::Zappyrang)
		Core::SetReadOnlyValue((char*)(Core::moduleBase + 0x5efb9), &newRang, 1);
}

void __fastcall E4MetalDoorMessage(GameObject* metalDoorActor, void* edx, BoomerangMKMessage* msg) {
	if (msg->MsgID == MSG_ExplosionMsg || (msg->MsgID == MSG_BoomerangMsg && (msg->HitRang->RangIndex == Rangs::Smasharang || msg->HitRang->RangIndex == Rangs::Kaboomerang)))
		Original_E4MetalDoorEvent(metalDoorActor, msg);
	//Don't run the rang event if it isn't the smasharang or kaboomerang
	else if (msg->MsgID != MSG_BoomerangMsg)
		Original_E4MetalDoorEvent(metalDoorActor, msg);
}

void Smasharang::HookSmasharangDamages()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x5f140), &SpyEggMessage, reinterpret_cast<LPVOID*>(&Original_SpyEggHit));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Spy Egg Damage Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x128410), &E4MetalDoorMessage, reinterpret_cast<LPVOID*>(&Original_E4MetalDoorEvent));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the E4 Metal Door Event Function Hook, With the Error: " + error, Error);
		return;
	}
}
