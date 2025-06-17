#include "ParticleSystem.h"

//TygerMemory
#include "core.h"

using TySystemDestroy_t = void(__thiscall*)(ParticleSystem* system);
TySystemDestroy_t TySystemDestroy = nullptr;
using TySystemDestroyDying_t = void(__thiscall*)(ParticleSystem* system, float deathTimer);
TySystemDestroyDying_t TySystemDestroyDying = nullptr;

void ParticleSystem::Destroy()
{
	if (!TySystemDestroy)
		TySystemDestroy = (TySystemDestroy_t)(Core::moduleBase + 0x1aac80);
	TySystemDestroy(this);
}

void ParticleSystem::Destroy(float deathTimer)
{
	if (!TySystemDestroyDying)
		TySystemDestroyDying = (TySystemDestroyDying_t)(Core::moduleBase + 0x1aac50);
	TySystemDestroyDying(this, deathTimer);
}
