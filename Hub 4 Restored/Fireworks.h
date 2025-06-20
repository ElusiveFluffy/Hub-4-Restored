#pragma once
#include "StaticProp.h"
#include "ParticleSystem.h"
#include "ShatterStruct.h"
#include <string>
#include <array>

enum class FireworksCrateState {
	Hidden,
	Visible,
	Burning,
	Dropping
};

struct FireworksParams {
	const std::string SubObjectName;
	const Vector4f NormalizedLaunchDir;
	
	float SinXOffset = 0.0f;
	float SinZOffset = 0.0f;
};

struct FireworkBurst {
	Model* Models[6] = {};
	float Scale = 0.0f;
	bool Exploding = false;
	int ExplosionTime = 0;

	void Init();
	void Deinit();
	void ExplodeUpdate();
	void Explode(Model* pCrateModel, FireworksParams* params);
};

struct Fireworks {
	int LaunchTime = 0;
	bool Exploded = false;
	Model* pModel = nullptr;
	FireworksParams Params[6] = {
		{ "F_Rocket", { -0.283869f, 0.948733f, -0.139013f, 1.0f } },
		{ "F_Rocket01", { 0.317213f, 0.938379f, -0.137185f, 1.0f } },
		{ "F_Rocket02", { 0.228182f, 0.802585f, 0.551172f, 1.0f } },
		{ "F_Rocket03", { -0.403097f, 0.802581f, 0.439746f, 1.0f } },
		{ "F_Rocket04", { -0.138725f, 0.904581f, -0.403098f, 1.0f } },
		{ "F_Rocket05", { 0.255547f, 0.929066f, -0.267454f, 1.0f } }
	};
	FireworkBurst Burst{};

	void Init(Model* pModel);
	void LaunchUpdate();
};

struct FireworksCrate : StaticProp {
	void Init(GameObjDesc* pDesc) override;
	void Deinit() override;
	void Message(MKMessage* pMsg) override;
	void Update() override;
	void Draw() override;

	FireworksCrateState State = FireworksCrateState::Visible;
	ParticleSystem* FireParticleSys = nullptr;
	ParticleSystem* FireGlowParticleSys = nullptr;
	ShatterStruct* FireworksShatter = nullptr;
	Fireworks Firework{};

	void Burn();
};

struct FireworksCrateDesc : StaticPropDesc {
	float LaunchSpeed = 14.0f;
	// Ty runs at a fixed update rate of 60 ticks a second
	int LaunchDuration = 60;
	// LaunchDuration + this
	int ShatterAdditionalDelay = 30;
	int BurstExplosionDuration = 75;

	std::array<RGBA, 13> BurstColours = {{
		// Warm tones
		{ 0.95f, 0.25f, 0.25f, 1.0f },  // Bright Red
		{ 1.0f, 0.5f, 0.0f, 1.0f },     // Orange
		{ 1.0f, 0.85f, 0.1f, 1.0f },    // Gold

		// Cool tones
		{ 0.0f, 0.8f, 0.2f, 1.0f },     // Emerald Green
		{ 0.1f, 0.6f, 1.0f, 1.0f },     // Sky Blue
		{ 0.4f, 0.2f, 1.0f, 1.0f },     // Violet
		{ 0.6f, 0.95f, 1.0f, 1.0f },    // Ice Blue

		// Neon
		{ 0.0f, 1.0f, 0.6f, 1.0f },     // Neon Green
		{ 0.9f, 0.1f, 0.9f, 1.0f },     // Magenta
		{ 1.0f, 0.2f, 0.6f, 1.0f },     // Hot Pink
		{ 0.6f, 1.0f, 0.2f, 1.0f },     // Lime
		{ 0.0f, 1.0f, 1.0f, 1.0f },     // Cyan

		{ 0.75f, 0.75f, 0.75f, 1.0f },  // Silver (Glitter)
	}};
};

namespace FireworksProp {
	inline FireworksCrateDesc FireworksDesc{};
	void InitGameObject(KromeIni* globalModel);
}
