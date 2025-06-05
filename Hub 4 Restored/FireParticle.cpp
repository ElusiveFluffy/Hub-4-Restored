#include "pch.h"
#include "FireParticle.h"

//TygerMemory
#include "core.h"

using TyParticle_Fire_Init_t = void(*)(ParticleSystem** system, Vector4f* particlePos, BoundingVolume* volume, float volumeScale, bool noFireGlow);
using TyParticle_Fire_Create_t = void(*)(ParticleSystem** system, Vector4f* spawnPos, float scale, bool noTransparency);

TyParticle_Fire_Init_t TyParticle_Fire_Init;
TyParticle_Fire_Create_t TyParticle_Fire_Create;

void FireParticle::Particle_Fire_Init(ParticleSystem** system, Vector4f* particlePos, BoundingVolume* volume, float volumeScale, bool noFireGlow)
{
	TyParticle_Fire_Init(system, particlePos, volume, volumeScale, noFireGlow);
}

void FireParticle::Particle_Fire_Create(ParticleSystem** system, Vector4f* spawnPos, float scale, bool noTransparency)
{
	TyParticle_Fire_Create(system, spawnPos, scale, noTransparency);
}

void FireParticle::InitFunctions()
{
	TyParticle_Fire_Init = (TyParticle_Fire_Init_t)(Core::moduleBase + 0x113950);
	TyParticle_Fire_Create = (TyParticle_Fire_Create_t)(Core::moduleBase + 0x1135c0);
}
