#include "pch.h"
#include "SmashCrate.h"
#include "TygerFrameworkAPI.hpp"
#include "Rangs.h"
#include "Hub4SFX.h"
#include "MinHook.h"
#include "TyFunctions.h"

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

GameObject::GetMKPropRange_t GetMKPropRange;

TyFunctions::VoidFunction_t OriginalCrate_CheckOpals;
GameObject::GameObjectMsg_t OriginalCrateMsg;

typedef void (*InitCrateGameObjects_t)(char* globalModel);
InitCrateGameObjects_t OriginalCrateObjectInit;

void Crate_CheckOpals() {
	//Run the Original Function for the wooden crate and B3 crate
	OriginalCrate_CheckOpals();

	//(Logic recreated from the game, was a bit simpler)
	//Maybe some valid state check, don't really know what it is, but the game checks this too
	if (*(bool*)(Core::moduleBase + 0x280418)) {
		CrateMKProp* crateProps[2];
		//Casts are required for a **, even though it inherits the base struct
		GetMKPropRange(&SmashCrate::GameObj, (GameObject::MKProp**)crateProps);
		CrateMKProp* crate = crateProps[0];

		while (crate < crateProps[1]) {
			if (crate == 0)
				break;

			int totalCollected = 0;

			if (crate->OpalCount > 0) {
				for (int i = 0; i < crate->OpalCount; i++) {
					OpalMKProp* opal = crate->OpalMKProps[i];

					if (opal && opal->State == Collected)
						totalCollected++;
				}
			}

			//Hide if all are collected
			if (crate->OpalCount == totalCollected) {
				crate->AllOpalsCollected = true;
				crate->Visible = false;
				crate->CollisionEnabled = false;
			}

			//Goes to the next instance, + 1 of its struct size
			crate += 1;
		}
	}
}

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
	bool smashCrate = _stricmp(aliasName, "SmashCrate") == 0;
	bool smashCrateRang = Rangs::GetCurrentRang() == Rangs::Smasharang || Rangs::GetCurrentRang() == Rangs::Kaboomerang;

	if (smashCrate && (msg->MsgID == ExplosionMsg || msg->MsgID == Break)) {
		//Change sfx
		Hub4SFX::GlobalSound newSound = Hub4SFX::SmashCrateSmash;
		Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &newSound, 4);

		//Now run the original function with the changed sfx
		OriginalCrateMsg(crate, msg);
		//Change the sfx back
		GlobalSound originalSound = GlobalSound::CrateSmash;
		Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &originalSound, 4);
	}
	else if (smashCrate && msg->MsgID == BoomerangMsg && smashCrateRang)
		OriginalCrateMsg(crate, msg);
	else if (msg->MsgID != BoomerangMsg || !smashCrate)
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

void HookEarlyInitFunction() {
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x60860), &InitCrateGameObjects, reinterpret_cast<LPVOID*>(&OriginalCrateObjectInit));
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x603f0), &Crate_CheckOpals, reinterpret_cast<LPVOID*>(&OriginalCrate_CheckOpals));
}

void SmashCrate::EarlyInit()
{
	HookEarlyInitFunction();

	InitCrateActor = (InitCrateActor_t)(Core::moduleBase + 0x13af00);
	InitGlobalModelStats = (InitGlobalModelStats_t)(Core::moduleBase + 0x13af80);
	SetPreviousGameObject = (SetPreviousGameObject_t)(Core::moduleBase + 0xf8cb0);

	GetMKPropRange = (GameObject::GetMKPropRange_t)(Core::moduleBase + 0xf8600);
}
