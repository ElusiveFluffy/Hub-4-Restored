#include "pch.h"
#include "SmashCrate.h"
#include "TygerFrameworkAPI.hpp"
#include "Rangs.h"
#include "Hub4GlobalSound.h"
#include "MinHook.h"

//TygerMemory
#include "core.h"

//Functions
typedef void(__thiscall* InitCrateActor_t)(GameObject::GameObjDesc* gameObjectDesc, GameObject::ModuleInfo* moduleInfo, const char* mdlName, const char* aliasName, int searchMask, int flags);
InitCrateActor_t InitCrateActor;
//Not 100% sure, but seems to set some things from global.model
typedef void(__thiscall* InitGlobalModelStats_t)(GameObject::GameObjDesc* gameObjectDesc, char* globalModel);
InitGlobalModelStats_t InitGlobalModelStats;
typedef void(__thiscall* SetPreviousGameObject_t)(GameObject::GameObjDesc** previousGameObjectDesc, GameObject::GameObjDesc* gameObjectDesc);
SetPreviousGameObject_t SetPreviousGameObject;

GameObjectMsg_t OriginalCrateMsg;

typedef void (*InitCrateGameObjects_t)(char* globalModel);
InitCrateGameObjects_t OriginalCrateObjectInit;

void InitCrateGameObjects(char* globalModel) {
	OriginalCrateObjectInit(globalModel);

	//Smashcrate
	//Need to manually set the vtable
	SmashCrate::GameObj.vTable = (GameObject::GameObjDescVTable*)(Core::moduleBase + 0x1fafd4);
	GameObject::ModuleInfo* crateModule = (GameObject::ModuleInfo*)(Core::moduleBase + 0x25520c);
	InitCrateActor(&SmashCrate::GameObj, crateModule, "P0934_SmashCrate", "SmashCrate", 0x41, 1);
	InitGlobalModelStats(&SmashCrate::GameObj, globalModel);
	SmashCrate::GameObj.StaticPropDesc.collisionInfoFlags = GameObject::NoIce;
	GameObject::GameObjDesc** previousGameObj = (GameObject::GameObjDesc**)(Core::moduleBase + 0x288738);
	SetPreviousGameObject(previousGameObj, &SmashCrate::GameObj);
}

void __fastcall CrateMsg(GameObject::MKProp* crate, void* edx, MKMessage* msg) {
	if (!crate->pDescriptor) {
		OriginalCrateMsg(crate, msg);
		return;
	}

	char* aliasName = crate->pDescriptor->AliasName;
	bool isSmashCrate = _stricmp(aliasName, "SmashCrate") == 0;
	bool Smashable = Rangs::GetCurrentRang() == Rangs::Smasharang || Rangs::GetCurrentRang() == Rangs::Kaboomerang;

	if (isSmashCrate && (msg->MsgID == ExplosionMsg || msg->MsgID == Break) && Smashable) {
		//Change sfx
		Hub4GlobalSound newSound = Hub4GlobalSound::SmashCrateSmash;
		Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &newSound, 4);

		//Now run the original function with the changed sfx
		OriginalCrateMsg(crate, msg);
		//Change the sfx back
		GlobalSound originalSound = GlobalSound::CrateSmash;
		Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &originalSound, 4);
	}
	else if (msg->MsgID == BoomerangMsg && Smashable)
		OriginalCrateMsg(crate, msg);
	else if (msg->MsgID != BoomerangMsg)
		OriginalCrateMsg(crate, msg);
}

//API Initialized, less time sensitive
void SmashCrate::HookFunctions()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x5f700), &CrateMsg, reinterpret_cast<LPVOID*>(&OriginalCrateMsg));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Crate Msg Function Hook, With the Error: " + error, Error);
		return;
	}
}

void HookInitFunction() {
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x60860), &InitCrateGameObjects, reinterpret_cast<LPVOID*>(&OriginalCrateObjectInit));
	if (minHookStatus != MH_OK) {
		return;
	}
}

void SmashCrate::EarlyInit()
{
	HookInitFunction();

	InitCrateActor = (InitCrateActor_t)(Core::moduleBase + 0x13af00);
	InitGlobalModelStats = (InitGlobalModelStats_t)(Core::moduleBase + 0x13af80);
	SetPreviousGameObject = (SetPreviousGameObject_t)(Core::moduleBase + 0xf8cb0);
}
