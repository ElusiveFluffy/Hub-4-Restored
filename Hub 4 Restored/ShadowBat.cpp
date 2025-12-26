#include "pch.h"
#include "GameObject.h"
#include "Fireworks.h"
#include "ShadowBat.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"
#include "level.h"

typedef Vector4f* (__thiscall* PipeGetFallingPos_t)(void* pPipe, Vector4f* fallPos);
typedef void(__thiscall* ShadowThiscall_t)(ShadowBatProp* shadowBat);
typedef bool(__thiscall* ShadowLoadLine_t)(ShadowBatProp* shadowBat, KromeIniLine* pLine);

PipeGetFallingPos_t Original_PipeGetFallingPos;
ShadowThiscall_t Original_FlyToNextPipeInit;
ShadowThiscall_t Original_Reset;
ShadowThiscall_t Original_Deinit;
ShadowLoadLine_t Original_LoadLine;
GameObjectInit_t Original_Init;
GameObjectMsg_t Original_Message;

// Just hook these bull functions, 
// as its much simpler then trying to recreate the logic to display the map icon
typedef Vector4f* (*Bull_GetPos_t)();
typedef Material* (*Bull_GetMat_t)();

Bull_GetPos_t Original_Bull_GetPos;
Bull_GetMat_t Original_Bull_GetMat;

Vector4f* ShadowBat_GetPos() {
	if (Vector4f* bullPos = Original_Bull_GetPos())
		return bullPos;

	ShadowBatProp* shadow = ShadowBatProp::GetShadowBat();
	// If shadow is active
	if (shadow && shadow->StaticPropFlags & 3) {
		return &shadow->pLocalToWorld->Position;
	}

	return nullptr;
}

Material* ShadowBat_GetMat() {
	if (Material* bullMat = Original_Bull_GetMat())
		return bullMat;

	ShadowBatProp* shadow = ShadowBatProp::GetShadowBat();
	// If shadow is active
	if (shadow && shadow->StaticPropFlags & 3) {
		return shadow->ShadowCameoMat;
	}

	return nullptr;
}

void __fastcall FlyToNextPipeInit(ShadowBatProp* shadowBat) {
	if (shadowBat->Health <= 0)
		return;
	if (FireworksProp::FireworksDesc.InstanceCount < shadowBat->Health)
	{
		API::LogPluginMessage("Not enough fireworks crates in the level!", Error);
	}
	else
	{
		// So the damage events increment up as more damage is taken
		switch (ShadowBat::ExtraEvents.MaxHP - shadowBat->Health) {
		case 1:
			ShadowBat::ExtraEvents.OnDamage1.Send();
			break;
		case 2:
			ShadowBat::ExtraEvents.OnDamage2.Send();
			break;
		case 3:
			ShadowBat::ExtraEvents.OnDamage3.Send();
			break;
		}
	}
	Original_FlyToNextPipeInit(shadowBat);
}

void __fastcall ShadowInit(GameObject* shadowBat, void* edx, GameObjDesc* pDesc) {
	Original_Init(shadowBat, pDesc);
	ShadowBat::ExtraEvents.OnDamage1.Init();
	ShadowBat::ExtraEvents.OnDamage2.Init();
	ShadowBat::ExtraEvents.OnDamage3.Init();
}

void __fastcall ShadowDeinit(ShadowBatProp* shadowBat) {
	Original_Deinit(shadowBat);
	// Just to make sure not to leave any dangling pointers behind
	ShadowBat::ExtraEvents.OnDamage1.Init();
	ShadowBat::ExtraEvents.OnDamage2.Init();
	ShadowBat::ExtraEvents.OnDamage3.Init();
}

void __fastcall ShadowMessage(GameObject* shadowBat, void* edx, MKMessage* msg) {
	if (msg->MsgID == MSG_Resolve) {
		ShadowBat::ExtraEvents.OnDamage1.Resolve();
		ShadowBat::ExtraEvents.OnDamage2.Resolve();
		ShadowBat::ExtraEvents.OnDamage3.Resolve();
	}
	Original_Message(shadowBat, msg);
}

bool __fastcall LoadLine(ShadowBatProp* shadowBat, void* edx, KromeIniLine* pLine) {
	// Run the original if the field is none of the on damage fields
	if (!ShadowBat::ExtraEvents.OnDamage1.LoadLine(pLine, "OnDamage1") &&
		!ShadowBat::ExtraEvents.OnDamage2.LoadLine(pLine, "OnDamage2") &&
		!ShadowBat::ExtraEvents.OnDamage3.LoadLine(pLine, "OnDamage3"))
		return Original_LoadLine(shadowBat, pLine);

	return true;
}

Vector4f* __fastcall PipeGetFallingPos(void* pPipe, void* edx, Vector4f* fallPos) {
	if (Level::getCurrentLevel() == LevelCode::B4)
	{
		*fallPos = { 0, 0, 0, 1 };
		return fallPos;
	}
	else
		return Original_PipeGetFallingPos(pPipe, fallPos);
}

void __fastcall Reset(ShadowBatProp* shadowBat) {
	ShadowBat::ExtraEvents.MaxHP = 3;
	const float* fallDeathTimePtr = (const float*)(Core::moduleBase + 0x1f9824);
	// For the 4 fireworks in the boss fight
	if (Level::getCurrentLevel() == LevelCode::B4)
	{
		ShadowBat::ExtraEvents.MaxHP = 4;
		fallDeathTimePtr = &ShadowBat::FallDeathExitSeconds;
	}

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xbf5a8), &ShadowBat::ExtraEvents.MaxHP, 4);

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc0e3a), &fallDeathTimePtr, 4);
	Original_Reset(shadowBat);
}

void HookFunction() {
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x13f070), &PipeGetFallingPos, reinterpret_cast<LPVOID*>(&Original_PipeGetFallingPos));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Pipe Get Falling Pos Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xc1140), &FlyToNextPipeInit, reinterpret_cast<LPVOID*>(&Original_FlyToNextPipeInit));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's fly to next pipe init Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf500), &Reset, reinterpret_cast<LPVOID*>(&Original_Reset));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Reset Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf7e0), &ShadowMessage, reinterpret_cast<LPVOID*>(&Original_Message));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Message Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf0f0), &ShadowInit, reinterpret_cast<LPVOID*>(&Original_Init));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Init Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf240), &ShadowDeinit, reinterpret_cast<LPVOID*>(&Original_Deinit));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Deinit Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xbf2a0), &LoadLine, reinterpret_cast<LPVOID*>(&Original_LoadLine));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Load Line Function Hook, With the Error: " + error, Error);
		return;
	}

	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x8dc70), &ShadowBat_GetPos, reinterpret_cast<LPVOID*>(&Original_Bull_GetPos));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Get Pos Function Hook, With the Error: " + error, Error);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x8dc20), &ShadowBat_GetMat, reinterpret_cast<LPVOID*>(&Original_Bull_GetMat));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Shadow's Get Mat Function Hook, With the Error: " + error, Error);
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
