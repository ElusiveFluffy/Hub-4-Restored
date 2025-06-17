#pragma once
#include "StaticProp.h"
#include "ParticleSystem.h"
#include "ShatterStruct.h"
#include <string>

enum class FireworksCrateState {
	Hidden,
	Visible,
	Burning,
	Dropping
};

struct FireworksParams {
	const std::string SubObjectName;
	const Vector4f NormalizedLaunchDir;
	Vector4f VelocityDir;
};

struct Fireworks {
	int LaunchTime = 0;
	bool Exploded = false;
	FireworksParams Params[6] = {
		{ "F_Rocket", { -0.283869f, 0.948733f, -0.139013f, 1.0f }, {}},
		{ "F_Rocket01", { 0.317213f, 0.938379f, -0.137185f, 1.0f }, {}},
		{ "F_Rocket02", { 0.228182f, 0.802585f, 0.551172f, 1.0f }, {}},
		{ "F_Rocket03", { -0.403097f, 0.802581f, 0.439746f, 1.0f }, {}},
		{ "F_Rocket04", { -0.138725f, 0.904581f, -0.403098f, 1.0f }, {}},
		{ "F_Rocket05", { 0.255547f, 0.929066f, -0.267454f, 1.0f }, {}}
	};
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
	BoundingVolume OriginalVolumeSize{};
	Fireworks Firework{};

	void Burn();
};

struct FireworksCrateDesc : StaticPropDesc {
	float LaunchSpeed = 10.0f;
	// Ty runs at a fixed update rate of 60 ticks a second
	int LaunchDuration = 60;
	// LaunchDuration + this
	int ShatterAdditionalDelay = 30;
};

namespace FireworksProp {
	inline FireworksCrateDesc FireworksDesc{};
	void InitGameObject(KromeIni* globalModel);
}
