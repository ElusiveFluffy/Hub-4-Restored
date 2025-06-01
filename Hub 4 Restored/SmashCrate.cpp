#include "pch.h"
#include "SmashCrate.h"
#include "TygerFrameworkAPI.hpp"
#include "Rangs.h"
#include "Hub4SFX.h"
#include "MinHook.h"
#include "TyFunctions.h"
#include "Fireworks.h"

//TygerMemory
#include "core.h"
#include "hero.h"

//Functions
typedef void(__thiscall* InitCrateActor_t)(GameObjDesc* gameObjectDesc, ModuleInfoBase* moduleInfo, const char* mdlName, const char* aliasName, int searchMask, int flags);
InitCrateActor_t InitCrateActor;
//Not 100% sure, but seems to set some things from global.model
GameObjDescLoad_t InitGlobalModelStats;

GetMKPropRange_t GetMKPropRange;

TyFunctions::VoidFunction_t OriginalCrate_CheckOpals;
GameObjectMsg_t OriginalCrateMsg;
GameObjectVoid_t OriginalCrateUpdate;

void Crate_CheckOpals() {
	//Run the Original Function for the wooden crate and B3 crate
	OriginalCrate_CheckOpals();

	//(Logic recreated from the game, was a bit simpler)
	//Maybe some valid state check, don't really know what it is, but the game checks this too
	if (*(bool*)(Core::moduleBase + 0x280418)) {
		CrateMKProp* crateProps[2];
		//Casts are required for a **, even though it inherits the base struct
		GetMKPropRange(&SmashCrate::GameObj, (GameObject**)crateProps);
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

void SmashCrate::InitGameObject(KromeIni* globalModel) {
	//Smashcrate
	//Need to manually set the vtable
	memcpy(&SmashCrate::GameObj, (void*)(Core::moduleBase + 0x254c40), 4);
	ModuleInfoBase* crateModule = (ModuleInfoBase*)(Core::moduleBase + 0x25520c);
	InitCrateActor(&SmashCrate::GameObj, crateModule, "P0934_SmashCrate", "SmashCrate", 0x41, 1);
	InitGlobalModelStats(&SmashCrate::GameObj, globalModel);
	SmashCrate::GameObj.collisionInfoFlags = NoIce;
	SetPreviousGameObject(GameObj::PreviousGameObj, &SmashCrate::GameObj);
}

bool UsingSmashCrateRang() {
	return Rangs::IsCurrentRang(Rangs::Smasharang) || Rangs::IsCurrentRang(Rangs::Kaboomerang);
}

void __fastcall CrateUpdate(CrateMKProp* crate) {
	if (!crate->pDescriptor) {
		OriginalCrateUpdate(crate);
		return;
	}
	
	char* aliasName = crate->pDescriptor->AliasName;
	bool smashCrate = _stricmp(aliasName, "SmashCrate") == 0;
	
	//Disable bite auto targetting on the smashcrates if not charge biting or rang targeting if not using a smashcrate rang
	if (smashCrate && !Hero::isChargeBiting()) {
		if (!UsingSmashCrateRang())
		{
			//Just completely jumps past the AutoTargetStruct::Set function
			BYTE jumpCode[2] = { 0xEB, 0x48 };
			Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x5f692), &jumpCode, 2);

			OriginalCrateUpdate(crate);

			//Restore the original opcode
			jumpCode[0] = 0x83; jumpCode[1] = 0xBE;
			Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x5f692), &jumpCode, 2);
			//No need to run the other code change
			return;
		}

		//Disable biting auto targeting (changes a je to a jmp so it always sets the bite target to a nullptr)
		BYTE jumpCode = 0xEB;
		Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x5f6cd), &jumpCode, 1);

		OriginalCrateUpdate(crate);

		//Restore the original opcode
		jumpCode = 0x74;
		Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x5f6cd), &jumpCode, 1);
	}
	else
		OriginalCrateUpdate(crate);
}

void __fastcall CrateMsg(CrateMKProp* crate, void* edx, MKMessage* msg) {
	if (!crate->pDescriptor) {
		OriginalCrateMsg(crate, msg);
		return;
	}

	char* aliasName = crate->pDescriptor->AliasName;
	bool smashCrate = _stricmp(aliasName, "SmashCrate") == 0;

	if (smashCrate) {
		//Stop the zappyrang's constant shatter message breaking the crate
		if (Rangs::IsCurrentRang(Rangs::Zappyrang) && !Hero::isChargeBiting() && msg->MsgID == MSG_Shatter)
			return;

		//Only not regular biting, charge biting is intended
		bool notBiting = Hero::getState() != (int)TyState::Biting || Hero::isChargeBiting();

		if ((msg->MsgID == MSG_ExplosionMsg || msg->MsgID == MSG_Shatter) && notBiting) {
			//Change sfx
			Hub4SFX::GlobalSound newSound = Hub4SFX::SmashCrateSmash;
			Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &newSound, 4);

			//Now run the original function with the changed sfx
			OriginalCrateMsg(crate, msg);
			//Change the sfx back
			GlobalSound originalSound = GlobalSound::CrateSmash;
			Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x5f791), &originalSound, 4);
		}
		else if (msg->MsgID == MSG_BoomerangMsg && UsingSmashCrateRang())
			OriginalCrateMsg(crate, msg);
		else if (msg->MsgID != MSG_BoomerangMsg && notBiting)
			OriginalCrateMsg(crate, msg);
	}
	else {
		//Do the regular crate logic if its not a smashcrate
		OriginalCrateMsg(crate, msg);
	}
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
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x5f580), &CrateUpdate, reinterpret_cast<LPVOID*>(&OriginalCrateUpdate));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Crate Update Function Hook, With the Error: " + error, Error);
		return;
	}
}

void HookEarlyInitFunction() {
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x603f0), &Crate_CheckOpals, reinterpret_cast<LPVOID*>(&OriginalCrate_CheckOpals));
}

void SmashCrate::EarlyInit()
{
	HookEarlyInitFunction();

	InitCrateActor = (InitCrateActor_t)(Core::moduleBase + 0x13af00);
	InitGlobalModelStats = (GameObjDescLoad_t)(Core::moduleBase + 0x13af80);
	SetPreviousGameObject = (SetPreviousGameObject_t)(Core::moduleBase + 0xf8cb0);

	GetMKPropRange = (GetMKPropRange_t)(Core::moduleBase + 0xf8600);
}
