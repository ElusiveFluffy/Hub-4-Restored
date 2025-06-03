#include "pch.h"
#include "Fireworks.h"
#include "ShadowBat.h"
#include "Rangs.h"
#include "TyPropFunctions.h"

ModuleInfo<FireworksMKProp> FireworksModule{};

void Fireworks::InitGameObject(KromeIni* globalModel)
{
	FireworksDesc.Init(&FireworksModule, "Prop_0225_Fireworks", "Fireworks", 0x41, 1);
	FireworksDesc.Load(globalModel);
	FireworksDesc.collisionInfoFlags = NoIce;
	SetPreviousGameObject(GameObj::PreviousGameObj, &FireworksDesc);
}

void FireworksMKProp::Message(MKMessage* pMsg)
{
	GameObject::Message(pMsg);

	if (pMsg->MsgID == MSG_BoomerangMsg && Rangs::IsCurrentRang(Rangs::Flamerang)) {
		ShadowBatProp* shadowProp = *(ShadowBatProp**)(Core::moduleBase + 0x25da18);
		if (shadowProp->StateManager.CurrentState != ShadowBat::Fall)
		{
			//Damages shadow
			shadowProp->StateManager.NextState = ShadowBat::Fall;
		}
	}
}

void FireworksMKProp::Update()
{
	if (Rangs::IsCurrentRang(Rangs::Flamerang))
		TyPropFunctions::AutoTargetSet(2, &pModel->Matrices.Position, nullptr, &pModel->Matrices.Position, pModel);
}
