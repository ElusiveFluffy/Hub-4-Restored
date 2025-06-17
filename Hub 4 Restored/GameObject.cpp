#include "pch.h"
#include "GameObject.h"
#include "StaticProp.h"
#include "Smashrock.h"
#include "SmashCrate.h"
#include "Fireworks.h"
#include "WaterTank.h"
#include "Windmill.h"
#include "ShadowBat.h"
#include "Rangs.h"
#include "MinHook.h"

//Just using the crate one since I already have the address from the hook with the smashcrate
typedef void (*InitGameObjects_t)(KromeIni* globalModel);
InitGameObjects_t OriginalInitGameObjects;

void InitGameObjects(KromeIni* globalModel) {
	OriginalInitGameObjects(globalModel);

	SmashCrate::InitGameObject(globalModel);
	FireworksProp::InitGameObject(globalModel);
}

// I assume the return values correspond to 0 when not within the frustum, 1 when it is, and -1 is when its not within the near and far plane maybe or maybe its occluded?
using WithinFrustum_t = int(*)(BoundingVolume* volume, Matrix* matrix, float nearPlane, float farPlane);
WithinFrustum_t Original_WithinFrustum = nullptr;

//Matrix is maybe the camera matrix its rendering from. The shadow and reflect thread also calls this
int WithinFrustum(BoundingVolume* bounds, Matrix* matrix, float nearPlane, float farPlane) {
	//Semi hacky way to not cull the fireworks sub objects when not looking at the main bounds when they shoot out
	if (FireworksProp::FireworksDesc.pInstances) {
		for (int i = 0; i < FireworksProp::FireworksDesc.InstanceCount; i++) {
			if (((FireworksCrate*)FireworksProp::FireworksDesc.pInstances)[i].State != FireworksCrateState::Burning)
				continue;
			// Uses the same pointer as the one in the model
			if (bounds == ((FireworksCrate*)FireworksProp::FireworksDesc.pInstances)[i].pModel->GetBoundingVolume(-1))
				return 1;
		}
	}
	return Original_WithinFrustum(bounds, matrix, nearPlane, farPlane);
}

void HookFunctions() {
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x60860), &InitGameObjects, reinterpret_cast<LPVOID*>(&OriginalInitGameObjects));

	MH_CreateHook((LPVOID*)(Core::moduleBase + 0xc6ba0), &WithinFrustum, reinterpret_cast<LPVOID*>(&Original_WithinFrustum));

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

	ShadowBat::InitFunctions();

	Rangs::HookRangFunctions();
	Rangs::RangSpecificSetup();
}

void ModuleInfoBase::Reset()
{
	pData->entityCount = 0;
	pData->Update = false;
}
