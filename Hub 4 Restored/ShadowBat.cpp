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
typedef void(__fastcall* ShadowFastcall_t)(ShadowBatProp* shadow);

PipeGetFallingPos_t Original_PipeGetFallingPos;
ShadowFastcall_t Original_FlyToNextPipeInit;
ShadowFastcall_t Original_Reset;

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
		FireworksCrate* crateProps = (FireworksCrate*)FireworksProp::FireworksDesc.pInstances;
		crateProps[shadowBat->Health - 1].DropCrate();
	}
	Original_FlyToNextPipeInit(shadowBat);
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
	int hp = 3;
	const float* fallDeathTimePtr = (const float*)(Core::moduleBase + 0x1f9824);
	//For the 4 fireworks in the boss fight
	if (Level::getCurrentLevel() == LevelCode::B4)
	{
		hp = 4;
		fallDeathTimePtr = &ShadowBat::FallDeathExitSeconds;

		if (FireworksProp::FireworksDesc.InstanceCount < hp)
		{
			API::LogPluginMessage("Not enough fireworks crates in the level!", Error);
		}
		else
		{
			FireworksCrate* crateProps = (FireworksCrate*)FireworksProp::FireworksDesc.pInstances;
			crateProps[hp - 1].Landed();
		}
	}

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xbf5a8), &hp, 4);

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
