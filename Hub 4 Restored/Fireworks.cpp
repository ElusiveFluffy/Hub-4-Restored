#include "pch.h"
#include "Fireworks.h"
#include "ShadowBat.h"
#include "Rangs.h"
#include "TyPropFunctions.h"
#include "ParticleFunctions.h"
#include "TyRandom.h"
#include "DebugDraw.h"
#include "MainRenderer.h"

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

	Firework.Init(pModel);
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
	Firework.Burst.Deinit();
	StaticProp::Deinit();
}

void FireworksCrate::Message(MKMessage* pMsg)
{
	if (pMsg->MsgID == MSG_BoomerangMsg && State != FireworksCrateState::Burning && ((BoomerangMKMessage*)pMsg)->HitRang->RangIndex == Rangs::Flamerang) {
		State = FireworksCrateState::Burning;

		if (!FireParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			ParticleFunctions::Particle_Fire_Init(&FireParticleSys, &pModel->Matrices[0].Position, volume, 3.0f, true);
		}

		if (!FireGlowParticleSys) {
			BoundingVolume* volume = pModel->GetBoundingVolume(-1);
			ParticleFunctions::Particle_Fire_Init(&FireGlowParticleSys, &pModel->Matrices[0].Position, volume, 3.0f, false);
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
		{
			Vector4f targetPos = pModel->Matrices[0].Position;
			// Roughly the center, otherwise it'll hit the ground sometimes when trying to target it
			targetPos.y += 35.0f;
			TyPropFunctions::AutoTargetSet(2, &targetPos, nullptr, &targetPos, pModel);
		}
		break;
	}
	case FireworksCrateState::Burning:
	{
		Burn();

		if (!Firework.Exploded)
		{
			Firework.LaunchUpdate();
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
			// The shatter manager has its own draw function to draw the shattering
			State = FireworksCrateState::Hidden;

			FireworksShatter->Explode((Vector4f*)(Core::moduleBase + 0x1fb2a0), 0.0599999987f, 3.0f);

			// Need to set them to false again as the shatter explode function sets them all to true
			for (FireworksParams& fireworkParam : Firework.Params)
				pModel->EnableSubObject(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()), false);

			Vector4f modelPos = pModel->Matrices[0].Position;
			Sound::PlayTySoundByIndex(GlobalSound::FireworksCrateExplodes, &modelPos);
		}
		Firework.LaunchTime++;

		break;
	}
	}
	if (Firework.Burst.Exploding)
		Firework.Burst.ExplodeUpdate();
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
	ParticleFunctions::Particle_Fire_Create(&FireParticleSys, &spawnPos, 2.0f, true);

	spawnPos.x = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.x);
	spawnPos.y = (TyRandom::RandomFR(0.0f, 65.0f) + modelPos.y);
	spawnPos.z = (TyRandom::RandomFR(0.0f, 70.0f) - 35.0f + modelPos.z);
	ParticleFunctions::Particle_Fire_Create(&FireGlowParticleSys, &spawnPos, 3.0f, false);
}

void Fireworks::Init(Model* modelPtr)
{
	pModel = modelPtr;

	float PI = 3.141592653589;
	for (FireworksParams& fireworkParam : Params)
	{
		fireworkParam.SinXOffset = TyRandom::RandomFR(0.0f, 2.0f * PI);
		fireworkParam.SinZOffset = TyRandom::RandomFR(0.0f, 2.0f * PI);
	}

	Burst.Init();
}

void Fireworks::LaunchUpdate()
{
	float timeElapsed = LaunchTime / 60.0f;
	float weaveStrength = 0.4f;
	float weaveFrequency = 7.0f;

	for (FireworksParams& fireworkParam : Params)
	{
		if (LaunchTime < fireworkParam.LaunchDelay)
			continue;
		Vector4f up = fireworkParam.NormalizedLaunchDir;

		// Local axes
		Vector4f tempForward = { 0.0f, 1.0f, 0.0f, 1.0f };
		Vector4f right = Vector4f::Normalize(Vector4f::Cross(tempForward, up));
		Vector4f forward = Vector4f::Normalize(Vector4f::Cross(up, right));

		// Offset in local right/forward directions using sine waves
		float offsetX = sinf(timeElapsed * weaveFrequency + fireworkParam.SinXOffset) * weaveStrength;
		float offsetZ = sinf(timeElapsed * weaveFrequency * 0.8f + fireworkParam.SinZOffset) * weaveStrength;

		Vector4f deviation = right * offsetX + forward * offsetZ;
		Vector4f weavingDir = Vector4f::Normalize(up + deviation);

		Vector4f velocity = weavingDir * FireworksProp::FireworksDesc.LaunchSpeed;

		int rocketMatrixIndex = pModel->GetSubObjectMatrixIndex(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()));
		pModel->Matrices[rocketMatrixIndex].Position = pModel->Matrices[rocketMatrixIndex].Position + velocity;
	}
	// The game only has a limited number of steam particles, so need to use them somewhat sparingly
	if (LaunchTime % 3 == 0) {
		int rocketMatrixIndex = pModel->GetSubObjectMatrixIndex(pModel->GetSubobjectIndex(Params[CurrentRocketParticleIndex].SubObjectName.c_str()));
		// Apply the base matrix to have the correct rotation and position
		Matrix rocketMatrix = pModel->Matrices[rocketMatrixIndex];
		rocketMatrix.Multiply(&rocketMatrix, pModel->pMatrices);
		Vector4f particlePos = rocketMatrix.Position;

		float scale = 47.0f;
		ParticleFunctions::SpawnWaterSteam(&particlePos, scale);

		CurrentRocketParticleIndex = (CurrentRocketParticleIndex + 1) % ARRAYSIZE(Params);
	}
	if (LaunchTime >= FireworksProp::FireworksDesc.LaunchDuration) {
		Exploded = true;
		ShadowBatProp* shadowProp = ShadowBatProp::GetShadowBat();
		if (shadowProp->StateManager.CurrentState != ShadowBat::Fall)
			shadowProp->Damage();

		for (FireworksParams& fireworkParam : Params)
			pModel->EnableSubObject(pModel->GetSubobjectIndex(fireworkParam.SubObjectName.c_str()), false);

		Burst.Explode(pModel, &Params[0]);
	}
}

void FireworkBurst::Init()
{
	int colourCount = FireworksProp::FireworksDesc.BurstColours.size();
	for (int i = 0; i < 6; i++) {
		Models[i] = Model::Create("FX07_FireworkBurst", nullptr);
		Models[i]->RenderType = 3;
		Models[i]->Colour = FireworksProp::FireworksDesc.BurstColours[TyRandom::RandomIR(0, colourCount)];
	}
	ExplodeEndFX = Material::Create("fx07_tx03");
	FallFX = Material::Create("fx07_tx04");
}

void FireworkBurst::Deinit()
{
	if (ExplodeFX) {
		Models[0]->pTemplate->pRenderEntry->pGLModelEntry->pMaterial = ExplodeFX;
	}
	for (int i = 0; i < 6; i++) {
		MainRenderer::RemoveModel(Models[i]);
		Models[i]->Destroy();
		Models[i] = nullptr;
	}
	if (ExplodeEndFX) {
		ExplodeEndFX->Destroy();
		ExplodeEndFX = nullptr;
	}
	if (FallFX) {
		FallFX->Destroy();
		FallFX = nullptr;
	}
}

void FireworkBurst::ExplodeUpdate()
{
	if (FireworksProp::FireworksDesc.BurstExplosionDuration - ExplosionTime < 15)
		for (int i = 0; i < 6; i++)
			Models[i]->Colour.A = (FireworksProp::FireworksDesc.BurstExplosionDuration - ExplosionTime) / 15.0f;
	if (ExplosionTime >= FireworksProp::FireworksDesc.BurstExplosionDuration) {
		Exploding = false;
		for (int i = 0; i < 6; i++)
			MainRenderer::RemoveModel(Models[i]);
		return;
	}

	ExplosionTime++;
	float ScaleRate = 0.2f / (ExplosionTime / 5.0f);
	Scale += ScaleRate;

	for (int i = 0; i < 6; i++) {
		Vector4f pos = Models[i]->pMatrices->Position;
		// Set Identity so it doesn't add to the existing scale
		Models[i]->pMatrices->SetIdentity();
		Models[i]->pMatrices->Scale(Models[i]->pMatrices, Scale);
		Models[i]->pMatrices->Position = pos;
		
		if (Phase == 1) {
			Models[i]->pMatrices->Position.y -= 1.0f;
		}
		else if (Phase == 2) {
			Models[i]->pMatrices->Position.y -= 5.0f;
		}
	}

	if (std::fmod((ExplosionTime / (FireworksProp::FireworksDesc.BurstExplosionDuration / 3.0f)), 1.0f) == 0.0f) {
		Phase++;
		switch (Phase) {
		case 1:
		{
			// Just need to edit model 0, since they all share model template data
			if (!ExplodeFX)
				ExplodeFX = Models[0]->pTemplate->pRenderEntry->pGLModelEntry->pMaterial;
			Models[0]->pTemplate->pRenderEntry->pGLModelEntry->pMaterial = ExplodeEndFX;
			break;
		}
		case 2:
		{
			Models[0]->pTemplate->pRenderEntry->pGLModelEntry->pMaterial = FallFX;
			Scale = 3.0f;
			for (int i = 0; i < 6; i++) {
				Vector4f pos = Models[i]->pMatrices->Position;
				// Set Identity so it doesn't add to the existing scale
				Models[i]->pMatrices->SetIdentity();
				Models[i]->pMatrices->Scale(Models[i]->pMatrices, Scale);
				Models[i]->pMatrices->Position = pos;
				Models[i]->pMatrices->Position.y -= 50.0f;
			}
			break;
		}
		case 3:
		{
			// Reset the material
			Models[0]->pTemplate->pRenderEntry->pGLModelEntry->pMaterial = ExplodeFX;
		}
		}
	}
}

void FireworkBurst::Explode(Model* pCrateModel, FireworksParams* params)
{
	Exploding = true;
	for (int i = 0; i < 6; i++) {
		int matrixIndex = pCrateModel->GetSubObjectMatrixIndex(pCrateModel->GetSubobjectIndex(params[i].SubObjectName.c_str()));
		memcpy(Models[i]->pMatrices, &pCrateModel->Matrices[matrixIndex], sizeof(Matrix));
		// The fireworks position is in local space relative to the crate, 
		// so it needs to be multiplied to apply the global positioning and rotation of the crate
		Models[i]->pMatrices->Multiply(Models[i]->pMatrices, pCrateModel->pMatrices);
		MainRenderer::AddModel(Models[i]);
	}

	Sound::PlayTySoundByIndex(GlobalSound::FireworksDetonate, &Models[0]->pMatrices->Position);
}
