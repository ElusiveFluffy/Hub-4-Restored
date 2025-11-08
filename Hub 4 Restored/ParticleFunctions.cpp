#include "pch.h"
#include "ParticleFunctions.h"

//TygerMemory
#include "core.h"

using TyParticle_Fire_Init_t = void(*)(ParticleSystem** system, Vector4f* particlePos, BoundingVolume* volume, float volumeScale, bool noFireGlow);
using TyParticle_Fire_Create_t = void(*)(ParticleSystem** system, Vector4f* spawnPos, float scale, bool noTransparency);

using TyParticleManager_SpawnWaterSteam_t = void(__thiscall*)(void* particleManager, Vector4f* spawnPos, float scale);

TyParticle_Fire_Init_t TyParticle_Fire_Init;
TyParticle_Fire_Create_t TyParticle_Fire_Create;

TyParticleManager_SpawnWaterSteam_t TySpawnWaterSteam;

void ParticleFunctions::Particle_Fire_Init(ParticleSystem** system, Vector4f* particlePos, BoundingVolume* volume, float volumeScale, bool noFireGlow)
{
	TyParticle_Fire_Init(system, particlePos, volume, volumeScale, noFireGlow);
}

void ParticleFunctions::Particle_Fire_Create(ParticleSystem** system, Vector4f* spawnPos, float scale, bool noTransparency)
{
	TyParticle_Fire_Create(system, spawnPos, scale, noTransparency);
}

void ParticleFunctions::SpawnWaterSteam(Vector4f* spawnPos, float scale)
{
	TySpawnWaterSteam(*(void**)(Core::moduleBase + 0x266ebc), spawnPos, scale);
}

void ParticleFunctions::InitFunctions()
{
	TyParticle_Fire_Init = (TyParticle_Fire_Init_t)(Core::moduleBase + 0x113950);
	TyParticle_Fire_Create = (TyParticle_Fire_Create_t)(Core::moduleBase + 0x1135c0);

	TySpawnWaterSteam = (TyParticleManager_SpawnWaterSteam_t)(Core::moduleBase + 0x118850);
}
