#pragma once
#include "StaticProp.h"
#include "ParticleSystem.h"

enum class FireworksCrateState {
	Hidden,
	Visible,
	Burning
};

struct FireworksMKProp : StaticProp {
	void Message(MKMessage* pMsg) override;
	void Update() override;
	void Draw() override;

	FireworksCrateState State = FireworksCrateState::Visible;
	int BurnTime = 0;
	ParticleSystem* FireParticleSys = nullptr;
	ParticleSystem* FireGlowParticleSys = nullptr;

	void Burn();
};

namespace Fireworks {
	inline StaticPropDesc FireworksDesc{};
	void InitGameObject(KromeIni* globalModel);
}
