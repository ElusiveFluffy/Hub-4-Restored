#pragma once
#include "ParticleSystem.h"

namespace FireParticle
{
	void Particle_Fire_Init(ParticleSystem** system, Vector4f* particlePos, BoundingVolume* volume, float volumeScale, bool noFireGlow);
	void Particle_Fire_Create(ParticleSystem** system, Vector4f* spawnPos, float arg3, bool noTransparency);

	void InitFunctions();
};
