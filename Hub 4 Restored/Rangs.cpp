#include "pch.h"
#include "Rangs.h"
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"
#include "NumUtil.h"
#include "SaveFile.h"

//TygerMemory
#include "core.h"

typedef Rangs::Boomerangs(*RangCycleFunction_t)(Rangs::Boomerangs currentRang);

Rangs::Boomerangs CycleForward(Rangs::Boomerangs currentRang) {
	Hub4SaveDataStruct* saveFile = SaveFile::GetHub4SaveData();

	int index = std::distance(Rangs::CycleOrder, std::find(std::begin(Rangs::CycleOrder), std::end(Rangs::CycleOrder), currentRang));
	Rangs::Boomerangs nextRang = Rangs::Boomerang;

	do {
		index = NumUtil::Wrap(index + 1, Rangs::RangCount - 1);
		nextRang = Rangs::CycleOrder[index];
		//Check if the rang is unlocked in the save file
	} while (*((bool*)&saveFile->AttributeData + (4 + nextRang)) == false);

	return nextRang;
}
Rangs::Boomerangs CycleBackward(Rangs::Boomerangs currentRang) {
	Hub4SaveDataStruct* saveFile = SaveFile::GetHub4SaveData();

	int index = std::distance(Rangs::CycleOrder, std::find(std::begin(Rangs::CycleOrder), std::end(Rangs::CycleOrder), currentRang));
	Rangs::Boomerangs previousRang = Rangs::Boomerang;

	do {
		index = NumUtil::Wrap(index - 1, Rangs::RangCount - 1);
		previousRang = Rangs::CycleOrder[index];
		//Check if the rang is unlocked in the save file
	} while (*((bool*)&saveFile->AttributeData + (4 + previousRang)) == false);

	return previousRang;
}

void Rangs::HookCycleFunctions()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x3c920), &CycleForward, nullptr);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Rang Cycle Forward Function Hook, With the Error: " + error, Error);
		return;
	}

	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x3c9c0), &CycleBackward, nullptr);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Rang Cycle Backward Function Hook, With the Error: " + error, Error);
		return;
	}

	//Enable the hooks
	minHookStatus = MH_EnableHook(MH_ALL_HOOKS);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Enable the Rang Cycle Function Hooks, With the Error: " + error, Error);
		return;
	}
}

Rangs::RangNameAndSoundData Rangs::ExtraRangNameAndSounds[] = {
	Rangs::RangNameAndSoundData { //Smasharang
		"prop_0490_rang_09",
		"RangStdThrow",
		"RangStdCatch",
		0,
		0,
		0,
		0x2e,
		0x38,
		TyRangSmashThrow,
		TyRangSmashCatch,
		TyRangSmashDeflect,
		TyRangSmashHit,
		TyJump
	},
};

Rangs::RangNameAndSoundData* NameAndSounds;

void RedirectRangModelAndSoundData() {
	NameAndSounds = new Rangs::RangNameAndSoundData[Rangs::RangCount];
	memcpy(NameAndSounds, (Rangs::RangNameAndSoundData*)(Core::moduleBase + 0x253668), sizeof(Rangs::RangNameAndSoundData) * Rangs::OriginalRangCount);

	memcpy(NameAndSounds + Rangs::RangCount - 1, &Rangs::ExtraRangNameAndSounds, sizeof(Rangs::RangNameAndSoundData));

	//Replace the pointers for it
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4ea), &NameAndSounds, 4);
	//For the rang wheel init
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c429), &NameAndSounds, 4);

	int ptr = (int)&NameAndSounds->RangNameTextIndex;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c439), &ptr, 4);

	ptr = (int)&NameAndSounds->unk4;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c409), &ptr, 4);
}

int* RangDataPointers[Rangs::RangCount];
//int* ExtraRangDataPointers;

Rangs::RangActorData* RangActorDataArray;

void RedirectRangDataPointers() {
	RangActorDataArray = new Rangs::RangActorData[Rangs::RangCount];

	//Game expects all the bytes to be 0, usually all 0xCD
	memset(RangActorDataArray, 0, sizeof(Rangs::RangActorData) * Rangs::RangCount);

	//int dataPointers = (int)&RangDataPointers;
	//Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c5c7), &dataPointers, 4);
	
	//Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x166c18), &dataPointers, 4);

	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x10f1), &RangActorDataArray, 4);

	int dataPointers = (int)&RangActorDataArray->NameAndSoundData;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4f8), &dataPointers, 4);

	dataPointers = (int)&RangActorDataArray->ModelName;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4f0), &dataPointers, 4);

	//The end address for a while loop
	dataPointers = ((int)RangActorDataArray + (sizeof(Rangs::RangActorData) * Rangs::RangCount));
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c5e3), &dataPointers, 4);
}

Rangs::RangGlowEffectParam* RangGlowArray;
//Stops any rendering bugs with the extra rangs, like everything turning black
void RedirectRangGlowArray() {
	RangGlowArray = new Rangs::RangGlowEffectParam[Rangs::RangCount];

	int rangGlowParamSize = sizeof(Rangs::RangGlowEffectParam);
	//Make sure its all 0
	memset(RangGlowArray, 0, rangGlowParamSize * Rangs::RangCount);

	memcpy(RangGlowArray, (Rangs::RangGlowEffectParam*)(Core::moduleBase + 0x2708e8), rangGlowParamSize * Rangs::OriginalRangCount);

	int dataPointer = (int)&RangGlowArray->HasGlow;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x16373e), &dataPointer, 4);
	//From a completely different function
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x16aaaf), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->TyGlowColour.R;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x163f22), &dataPointer, 4);
	dataPointer = (int)&RangGlowArray->TyGlowColour.G;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x163f33), &dataPointer, 4);
	dataPointer = (int)&RangGlowArray->TyGlowColour.B;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x163f40), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->PulseChangeRate;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x163f6f), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->GroundGlowFalloff;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x164258), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->GroundGlowColour.R;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x16424b), &dataPointer, 4);
	dataPointer = (int)&RangGlowArray->GroundGlowColour.G;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x164267), &dataPointer, 4);
	dataPointer = (int)&RangGlowArray->GroundGlowColour.B;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x164284), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->EntireLvlGlowIntensity;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x164290), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->GroundGlowIntensity;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x1642a2), &dataPointer, 4);

	dataPointer = (int)&RangGlowArray->GroundGlowRadius;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x1642bb), &dataPointer, 4);
}

void SetRangCycleOrder() {
	int customCyleOrderPtr = (int)&Rangs::CycleOrder;

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3c149), &customCyleOrderPtr, 4);

	//The rest is simpler with a hook and less time sensitive (and required to change the amount of rangs)
}

Rangs::RangPropertyFunctions** RangPropertySetupFunctions;

void Rangs::SetupRangStructs()
{
	RangPropertySetupFunctions = new RangPropertyFunctions*[RangCount];
	memcpy(RangPropertySetupFunctions, (RangPropertyFunctions*)(Core::moduleBase + 0x253920), sizeof(RangPropertyFunctions*) * RangCount);

	RangPropertySetupFunctions[12] = RangPropertySetupFunctions[0];


	int* codeRangPropertyFunctionsPtr = (int*)(Core::moduleBase + 0x3c511);
	Core::SetReadOnlyValue(codeRangPropertyFunctionsPtr, &RangPropertySetupFunctions, 4);

	Core::SetReadOnlyValue(codeRangPropertyFunctionsPtr, &RangPropertySetupFunctions, 4);

	//Set Rang Count
	int* codeRangCount = (int*)(Core::moduleBase + 0x166c1f);
	Core::SetReadOnlyValue(codeRangCount, (int*)&RangCount, 4);

	//Rang Function
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x10f6), (int*)&RangCount, 4);

	SetRangCycleOrder();

	RedirectRangDataPointers();
	RedirectRangGlowArray();
	RedirectRangModelAndSoundData();
}
