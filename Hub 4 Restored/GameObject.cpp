#include "pch.h"
#include "GameObject.h"
#include "StaticProp.h"
#include "Smashrock.h"
#include "SmashCrate.h"
#include "Fireworks.h"
#include "WaterTank.h"
#include "Windmill.h"
#include "Rangs.h"
#include "MinHook.h"

//Just using the crate one since I already have the address from the hook with the smashcrate
typedef void (*InitGameObjects_t)(KromeIni* globalModel);
InitGameObjects_t OriginalInitGameObjects;

void InitGameObjects(KromeIni* globalModel) {
	OriginalInitGameObjects(globalModel);

	SmashCrate::InitGameObject(globalModel);
	Fireworks::InitGameObject(globalModel);
}

void HookFunctions() {
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x60860), &InitGameObjects, reinterpret_cast<LPVOID*>(&OriginalInitGameObjects));

	WaterTank::HookFunctions();
	Windmill::HookFunctions();
}

void GameObj::EarlyInit()
{
	Rangs::SetupRangStructs();

	//Make sure to set these first for anything the uses them afterwards
	AddToModuleList = (AddToModuleList_t)(Core::moduleBase + 0xf85c0);

	PreviousGameObj = (GameObjDesc**)(Core::moduleBase + 0x288738);

	//Game object/MK prop default vtable functions
	DefaultLoadLine = (GameObjectLoadLine_t)(Core::moduleBase + 0xf8910);
	DefaultDraw = (GameObjectVoid_t)(Core::moduleBase + 0xf86b0);
	DefaultMessage = (GameObjectMsg_t)(Core::moduleBase + 0xf8b40);
	DefaultInit = (GameObjectInit_t)(Core::moduleBase + 0xf87a0);
	DefaultDeinit = (GameObjectVoid_t)(Core::moduleBase + 0xf8690);

	//Game object desc vtable functions
	DefaultGameObjDescInit = (GameObjDescInit_t)(Core::moduleBase + 0xf8710);
	DefaultGameObjDescLoad = (GameObjDescLoad_t)(Core::moduleBase + 0xf8800);

	HookFunctions();

	StaticProps::Init();

	SmashCrate::EarlyInit();
}

void GameObj::APIInit()
{
	Smashrock::SmashrockInit();
	SmashCrate::HookFunctions();

	Rangs::HookRangFunctions();
	Rangs::RangSpecificSetup();
}

void ModuleInfoBase::Reset()
{
	pData->entityCount = 0;
	pData->Update = false;
}
