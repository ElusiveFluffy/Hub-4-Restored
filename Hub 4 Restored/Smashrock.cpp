#include "pch.h"
#include "Smashrock.h"
using namespace Smashrock;
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"
#include "GameObject.h"

//TygerMemory
#include "core.h"
#include "sound.h"

//Array with size of 3
//Index 0: Ice wall
//Index 1: Fire logs
//Index 2: Smash rock
Rangs::Boomerangs* RCBlockageValidRangs = nullptr;

typedef UINT* (__fastcall* PlayShatterableSound_t)(MKProp* prop);
PlayShatterableSound_t Original_PlayShatterableSound;

//To make the smashrock work with both the smasharang and kaboomerang
void Smashrock::UpdateSmashrockValidRang(Rangs::Boomerangs newRang)
{
	if (newRang == Rangs::Smasharang || newRang == Rangs::Kaboomerang)
		RCBlockageValidRangs[2] = newRang;
}

UINT* __fastcall PlayShatterableSound(MKProp* prop) {
	if (!prop->pDescriptor)
		return Original_PlayShatterableSound(prop);

	char* aliasName = prop->pDescriptor->AliasName;

	//The normal function doesn't have a entry for the smashrock
	//stricmp returns 0 if they're the same
	if (_stricmp(aliasName, "SMASHROCK") == 0)
		return Sound::PlayTySoundByIndex(GlobalSound::BoulderSmash, &prop->pModel->pMatrices->Position);

	return Original_PlayShatterableSound(prop);
}

void HookShatterFunction() {
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x1403d0), &PlayShatterableSound, reinterpret_cast<LPVOID*>(&Original_PlayShatterableSound));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Play Shatterable Sound Function Hook, With the Error: " + error, Error);
		return;
	}
}

void Smashrock::SmashrockInit()
{
	RCBlockageValidRangs = (Rangs::Boomerangs*)(Core::moduleBase + 0x26eb20);

	HookShatterFunction();
}
