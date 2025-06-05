#include "pch.h"
#include "Fireworks.h"
#include "ShadowBat.h"
#include "Rangs.h"
#include "TyPropFunctions.h"
#include "FireParticle.h"
#include "TyRandom.h"

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
	if (pMsg->MsgID == MSG_BoomerangMsg && ((BoomerangMKMessage*)pMsg)->HitRang->RangIndex == Rangs::Flamerang) {
		ShadowBatProp* shadowProp = *(ShadowBatProp**)(Core::moduleBase + 0x25da18);
		if (shadowProp->StateManager.CurrentState != ShadowBat::Fall)
		{
			shadowProp->Damage();
		}
		State = FireworksCrateState::Burning;

		if (!FireParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			FireParticle::Particle_Fire_Init(&FireParticleSys, &pModel->Matrices.Position, volume, 3.0f, true);
		}

		if (!FireGlowParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			FireParticle::Particle_Fire_Init(&FireGlowParticleSys, &pModel->Matrices.Position, volume, 3.0f, false);
		}
	}
	GameObject::Message(pMsg);
}

void FireworksMKProp::Update()
{
	if (Rangs::IsCurrentRang(Rangs::Flamerang))
		TyPropFunctions::AutoTargetSet(2, &pModel->Matrices.Position, nullptr, &pModel->Matrices.Position, pModel);

	if (State == FireworksCrateState::Burning) {
		Vector4f modelPos = pModel->Matrices.Position;
		Vector4f spawnPos{};
		spawnPos.x = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.x);
		spawnPos.y = (TyRandom::RandomFR(0.0f, 65.0f) + modelPos.y);
		spawnPos.z = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.z);
		spawnPos.w = modelPos.w;
		FireParticle::Particle_Fire_Create(&FireParticleSys, &spawnPos, 2.0f, true);

		spawnPos.x = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.x);
		spawnPos.y = (TyRandom::RandomFR(0.0f, 65.0f) + modelPos.y);
		spawnPos.z = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.z);
		FireParticle::Particle_Fire_Create(&FireGlowParticleSys, &spawnPos, 3.0f, false);
	}
}

void FireworksMKProp::Draw()
{
	if (State == FireworksCrateState::Hidden)
		return;
	StaticProp::Draw();
}

void FireworksMKProp::Burn()
{

}
