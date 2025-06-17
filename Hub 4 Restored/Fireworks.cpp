#include "pch.h"
#include "Fireworks.h"
#include "ShadowBat.h"
#include "Rangs.h"
#include "TyPropFunctions.h"
#include "FireParticle.h"
#include "TyRandom.h"
#include "DebugDraw.h"

//TygerMemory
#include "sound.h"

ModuleInfo<FireworksCrate> FireworksModule{};

void FireworksProp::InitGameObject(KromeIni* globalModel)
{
	FireworksDesc.Init(&FireworksModule, "Prop_0225_Fireworks", "Fireworks", 0x41, 1);
	FireworksDesc.Load(globalModel);
	FireworksDesc.collisionInfoFlags = NoIce;
	SetPreviousGameObject(GameObj::PreviousGameObj, &FireworksDesc);
}

void FireworksCrate::Init(GameObjDesc* pDesc)
{
	StaticProp::Init(pDesc);
	FireworksShatter = Shatter_Add(pModel, 0.349999994f, 0.449999988f, 120);
	FireworksShatter->OnFire = true;
}

void FireworksCrate::Deinit()
{
	if (FireParticleSys)
	{
		FireParticleSys->Destroy();
		FireParticleSys = nullptr;
	}
	if (FireGlowParticleSys)
	{
		FireGlowParticleSys->Destroy();
		FireGlowParticleSys = nullptr;
	}
	StaticProp::Deinit();
}

void FireworksCrate::Message(MKMessage* pMsg)
{
	if (pMsg->MsgID == MSG_BoomerangMsg && State != FireworksCrateState::Burning && ((BoomerangMKMessage*)pMsg)->HitRang->RangIndex == Rangs::Flamerang) {
		ShadowBatProp* shadowProp = ShadowBatProp::GetShadowBat();
		State = FireworksCrateState::Burning;

		if (!FireParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			FireParticle::Particle_Fire_Init(&FireParticleSys, &pModel->Matrices[0].Position, volume, 3.0f, true);
		}

		if (!FireGlowParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			FireParticle::Particle_Fire_Init(&FireGlowParticleSys, &pModel->Matrices[0].Position, volume, 3.0f, false);
		}

		Sound::PlayTySoundByIndex(GlobalSound::FireworksIgnite, &pModel->Matrices[0].Position, 0);
		Sound::PlayTySoundByIndex(GlobalSound::FireworksLaunch, &pModel->Matrices[0].Position, 0);
	}
	GameObject::Message(pMsg);
}

void FireworksCrate::Update()
{
	switch (State) {
	case FireworksCrateState::Visible:
	{
		if (Rangs::IsCurrentRang(Rangs::Flamerang))
			TyPropFunctions::AutoTargetSet(2, &pModel->Matrices[0].Position, nullptr, &pModel->Matrices[0].Position, pModel);
		break;
	}
	case FireworksCrateState::Burning:
	{
		Burn();
		Vector4f modelPos = pModel->Matrices[0].Position;

		if (!Firework.Exploded)
		{
			for (FireworksParams& fireworkParam : Firework.Params)
			{
				int rocketMatrixIndex = pModel->GetSubObjectMatrixIndex(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()));
				Vector4f dir = fireworkParam.NormalizedLaunchDir * FireworksProp::FireworksDesc.LaunchSpeed;
				pModel->Matrices[rocketMatrixIndex].Position = pModel->Matrices[rocketMatrixIndex].Position + dir;
			}
			if (Firework.LaunchTime >= FireworksProp::FireworksDesc.LaunchDuration) {
				Firework.Exploded = true;
				ShadowBatProp* shadowProp = ShadowBatProp::GetShadowBat();
				if (shadowProp->StateManager.CurrentState != ShadowBat::Fall)
					shadowProp->Damage();

				for (FireworksParams& fireworkParam : Firework.Params)
					pModel->EnableSubObject(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()), false);

				Sound::PlayTySoundByIndex(GlobalSound::FireworksDetonate, &modelPos);
			}
		}
		else if (Firework.LaunchTime >= FireworksProp::FireworksDesc.LaunchDuration + FireworksProp::FireworksDesc.ShatterAdditionalDelay) {
			if (FireParticleSys)
			{
				FireParticleSys->Destroy(1.0f);
				FireParticleSys = nullptr;
			}
			if (FireGlowParticleSys)
			{
				FireGlowParticleSys->Destroy(1.0f);
				FireGlowParticleSys = nullptr;
			}

			collisionInfo.Enabled = false;
			// The shatter struct has its own draw function to draw the shattering
			State = FireworksCrateState::Hidden;

			FireworksShatter->Explode((Vector4f*)(Core::moduleBase + 0x1fb2a0), 0.0599999987f, 3.0f);

			// Need to set them to false again as the shatter explode function sets them all to true
			for (FireworksParams& fireworkParam : Firework.Params)
				pModel->EnableSubObject(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()), false);
			Sound::PlayTySoundByIndex(GlobalSound::FireworksCrateExplodes, &modelPos);
		}
		Firework.LaunchTime += 1;

		break;
	}
	}
}

void FireworksCrate::Draw()
{
	if (State == FireworksCrateState::Hidden)
		return;
	StaticProp::Draw();
}

void FireworksCrate::Burn()
{
	Vector4f modelPos = pModel->Matrices[0].Position;
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
