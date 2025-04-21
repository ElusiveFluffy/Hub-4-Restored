#include "pch.h"
#include "Fireworks.h"

ModuleInfo<FireworksMKProp> FireworksModule{};

void Fireworks::InitGameObject(KromeIni* globalModel)
{
	FireworksDesc.Init(&FireworksModule, "Prop_0225_Fireworks", "Fireworks", 1, 1);
	FireworksDesc.Load(globalModel);
	FireworksDesc.collisionInfoFlags = NoIce;
	SetPreviousGameObject(GameObj::PreviousGameObj, &FireworksDesc);
}

void FireworksMKProp::Message(MKMessage* pMsg)
{
	GameObject::Message(pMsg);
}
