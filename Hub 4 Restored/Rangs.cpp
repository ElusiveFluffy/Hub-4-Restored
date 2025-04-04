#include "pch.h"
#include "Rangs.h"
using namespace Rangs;
#include "MinHook.h"
#include "TygerFrameworkAPI.hpp"
#include "NumUtil.h"
#include "SaveFile.h"
#include "Smashrock.h"

#include "Flamerang.h"
#include "Smasharang.h"

//TygerMemory
#include "core.h"

typedef Boomerangs(__thiscall* SetCurrentRang_t)(void* boomerangManager, Boomerangs newRang);
SetCurrentRang_t Original_SetCurrentRang;

//To not add an extra arg to the cycle functions used with a hook, which probably wouldn't work
bool CycleDontIncludeAqua = true;

Boomerangs CycleForward(Boomerangs currentRang) {
	Hub4SaveDataStruct* saveFile = SaveFile::GetHub4SaveData();

	int index = std::distance(CycleOrder, std::find(std::begin(CycleOrder), std::end(CycleOrder), currentRang));
	Boomerangs nextRang = Ironbark;

	do {
		index = NumUtil::Wrap(index + 1, RangCount - CycleDontIncludeAqua);
		nextRang = CycleOrder[index];
		//Check if the rang is unlocked in the save file
	} while (*((bool*)&saveFile->AttributeData + (4 + nextRang)) == false);

	return nextRang;
}
Boomerangs CycleBackward(Boomerangs currentRang) {
	Hub4SaveDataStruct* saveFile = SaveFile::GetHub4SaveData();

	int index = std::distance(CycleOrder, std::find(std::begin(CycleOrder), std::end(CycleOrder), currentRang));
	Boomerangs previousRang = Ironbark;

	do {
		index = NumUtil::Wrap(index - 1, RangCount - CycleDontIncludeAqua);
		previousRang = CycleOrder[index];
		//Check if the rang is unlocked in the save file
	} while (*((bool*)&saveFile->AttributeData + (4 + previousRang)) == false);

	return previousRang;
}

Boomerangs RangInfoCycleForward() {
	Sound::PlayTySoundByIndex(GlobalSound::FrontendClick);

	Boomerangs* currentRangInfoRang = (Boomerangs*)(Core::moduleBase + 0x288394);
	CycleDontIncludeAqua = false;
	//Need to set it to this variable
	*currentRangInfoRang = CycleForward(*currentRangInfoRang);
	CycleDontIncludeAqua = true;

	//The game usually returns it too, even though it doesn't do anything with the return value I think
	return *currentRangInfoRang;
}

Boomerangs RangInfoCycleBackward() {
	Sound::PlayTySoundByIndex(GlobalSound::FrontendClick);

	Boomerangs* currentRangInfoRang = (Boomerangs*)(Core::moduleBase + 0x288394);
	CycleDontIncludeAqua = false;
	//Need to set it to this variable
	*currentRangInfoRang = CycleBackward(*currentRangInfoRang);
	CycleDontIncludeAqua = true;

	//The game usually returns it too, even though it doesn't do anything with the return value I think
	return *currentRangInfoRang;
}

// much easier to use __fastcall as a detour for __thiscall as it works similarly to __thiscall, without need it to be a class member (which minhook doesn't seem to like),
// but it also uses edx, which is unused in this case as __thiscall only uses one register(ecx) for the first value which is the "this" of the class member, 
// the other args afterwards are on the stack
Boomerangs __fastcall CurrentRangChanged(void* boomerangManager, void* edx, Boomerangs newRang) {
	Smashrock::UpdateSmashrockValidRang(newRang);
	return Original_SetCurrentRang(boomerangManager, newRang);
}

void Rangs::HookRangFunctions()
{
	//Cycle functions
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

	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xf3500), &RangInfoCycleForward, nullptr);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Rang Info Cycle Forward Function Hook, With the Error: " + error, Error);
		return;
	}

	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xf3540), &RangInfoCycleBackward, nullptr);
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Rang Info Cycle Backward Function Hook, With the Error: " + error, Error);
		return;
	}

	//Set rang function
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x3f8c0), &CurrentRangChanged, reinterpret_cast<LPVOID*>(&Original_SetCurrentRang));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the Rang Changed Function Hook, With the Error: " + error, Error);
		return;
	}
}

Boomerangs Rangs::GetCurrentRang()
{
	return *(Boomerangs*)(Core::moduleBase + 0x2719A0);
}

Rangs::BoomerangStaticInfo Rangs::ExtraBoomerangInfo[] = {
	{ //Smasharang
		"prop_0490_rang_09",
		"RangStdThrow",
		"RangStdCatch",
		0,
		0,
		0,
		0x2e,
		0x3c,
		GlobalSound::TyRangSmashThrow,
		GlobalSound::TyRangSmashCatch,
		GlobalSound::TyRangSmashDeflect,
		GlobalSound::TyRangSmashHit,
		GlobalSound::TyJump
	},
};

Rangs::BoomerangStaticInfo* BoomerangInfo;

void RedirectRangModelAndSoundData() {
	BoomerangInfo = new Rangs::BoomerangStaticInfo[Rangs::RangCount];
	memcpy(BoomerangInfo, (Rangs::BoomerangStaticInfo*)(Core::moduleBase + 0x253668), sizeof(Rangs::BoomerangStaticInfo) * Rangs::OriginalRangCount);

	memcpy(BoomerangInfo + Rangs::RangCount - 1, &Rangs::ExtraBoomerangInfo, sizeof(Rangs::BoomerangStaticInfo));

	//Replace the pointers for it
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4ea), &BoomerangInfo, 4);
	//For the rang wheel init
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c429), &BoomerangInfo, 4);

	int ptr = (int)&BoomerangInfo->RangNameTextIndex;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c439), &ptr, 4);

	ptr = (int)&BoomerangInfo->RangDescTextIndex;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c409), &ptr, 4);
}

Rangs::BoomerangDescriptor* BoomerangDescriptorArray;

void RedirectRangDataPointers() {
	BoomerangDescriptorArray = new Rangs::BoomerangDescriptor[Rangs::RangCount];

	//Game expects all the bytes to be 0, usually all 0xCD
	memset(BoomerangDescriptorArray, 0, sizeof(Rangs::BoomerangDescriptor) * Rangs::RangCount);

	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x10f1), &BoomerangDescriptorArray, 4);

	int dataPointers = (int)&BoomerangDescriptorArray->NameAndSoundData;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4f8), &dataPointers, 4);

	dataPointers = (int)&BoomerangDescriptorArray->ModelName;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x3c4f0), &dataPointers, 4);

	//The end address for a while loop
	dataPointers = ((int)BoomerangDescriptorArray + (sizeof(Rangs::BoomerangDescriptor) * Rangs::RangCount));
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

void* RangInfoModels;

void RedirectRangInfoModelPtrs() {
	RangInfoModels = new void*[RangCount];

	//Game info cycling model pointers
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe4480), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe448b), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe44ae), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe44ba), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe44cd), &RangInfoModels, 4);

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf315a), &RangInfoModels, 4);

	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf35ee), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf3614), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf362f), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf364e), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf3677), &RangInfoModels, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf368e), &RangInfoModels, 4);

	//Rang count
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xe44de), (int*)&RangCount, 1);
}

//1 rang count used for controller is stored as a float
float FloatRangCount;

void SetupRangWheel() {
	//Rang model init count
	int wheelRangCount = RangCount - 1; //-1 to not include the aquarang
	//Init
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3d955), &wheelRangCount, 1);
	//Rang wheel
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3e3e8), &wheelRangCount, 1);

	//Maybe unneeded but just in case
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dfbd), &wheelRangCount, 1);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dfe2), &wheelRangCount, 1);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dfe5), &wheelRangCount, 1);
	//Selection
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dde7), &wheelRangCount, 1);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dbff), &wheelRangCount, 1);

	//Controller selection
	FloatRangCount = wheelRangCount;
	float* floatRangCountPtr = &FloatRangCount;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3df60), &floatRangCountPtr, 4);

	//Opening rang wheel
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3d0ac), &wheelRangCount, 1);

	//Maybe also unneeded but just in case
	int wheelMaxIndex = wheelRangCount - 1;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3dc25), &wheelMaxIndex, 4);

	float rangSpacingRadians = 6.28318f / wheelRangCount;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x1fa23c), &rangSpacingRadians, 4);

	//Rang model array size
	int rangPtrArraySize = wheelRangCount * 4;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3d4fb), &rangPtrArraySize, 1);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3d42a), &rangPtrArraySize, 1);
}

void EnableOneRangCycling() {
	//Easily place a certain amount of nops based on the amount of bytes set
	BYTE nops[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

	//Allow rang wheel to show when idle
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x25f7a), &nops, 2); //(Maybe not required)
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x2e620), &nops, 2);

	//Cycle rangs and allow rang wheel to show while moving
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x162b9c), &nops, 6);

	//Cycle rangs while idle
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x2e5be), &nops, 2);

	//Weird doomerang state change thing that sets Ty's state to idle when you only have one rang and cycle to the doomerang for some reason
	//Seems to maybe be for changing Ty's state back to idle after throwing the doomerang
	//The collection state condition doesn't work properly anyways, so check if it is equal to 50 instead of not equal to 33
	BYTE conditionByte = 50;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3bedb), &conditionByte, 1);
	//Change from je to jne
	conditionByte = 0x75;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x3bedc), &conditionByte, 1);
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

	EnableOneRangCycling();

	SetRangCycleOrder();
	RedirectRangInfoModelPtrs();

	SetupRangWheel();

	RedirectRangDataPointers();
	RedirectRangGlowArray();
	RedirectRangModelAndSoundData();
}

//Set up stuff more specific to individual rangs
void Rangs::RangSpecificSetup()
{
	Smasharang::HookSmasharangDamages();
	Flamerang::ShortenCrateBurnTime();
}