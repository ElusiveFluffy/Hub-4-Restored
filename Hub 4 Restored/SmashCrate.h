#pragma once
#include "StaticProp.h"
#include "ShatterStruct.h"
#include "ParticleSystem.h"

#include "vector4f.h"

enum OpalState
{
    Despawned = 0x0,
    Spawning = 0x1,
    Resting = 0x2,
    Magnetized = 0x3,
    SavingPosition = 0x4,
    Collected = 0x5
};
enum class CrateState
{
    Hidden = 0x0,
    Visible = 0x1,
    Burning = 0x4,
    Hiding = 0x5
};


struct OpalMKProp : GameObject
{
    char _44[0x34];
    OpalState State;
    Vector4f Position;
    char _8c[4];
    char _90[8];
    float LerpTime;
    Vector4f CollectPos;
    Vector4f CollisionNormal;
    Matrix field_bc;
    int16_t field_fc;
    char field_fe;
    char _ff[1];
    char _100[0x14];
};
struct CrateMKProp : StaticProp
{
    char _60[0x14];
    char _74[0xc];
    char _80[4];
    char _84[4];
    char _88[4];
    char _8c[8];
    char _94[4];
    char _98[4];
    char _9c[0x24];
    char _c0[0x1c];
    ParticleSystem* FireParticleSys;
    ParticleSystem* FireGlowParticleSys;
    char _e4[4];
    char _e8[8];
    char _f0[4];
    char _f4[4];
    char _f8[4];
    char _fc[1];
    char _fd[3];
    char _100[4];
    char _104[4];
    char _108[4];
    char _10c[2];
    char _10e[2];
    char _110[4];
    CrateState State;
    bool AllOpalsCollected;
    char _11c[4];
    char _120[4];
    char _124[4];
    char _128[4];
    char _12c[4];
    int BurnTime;
    char _134[4];
    char _138[4];
    int CrateType; // Maybe
    ShatterStruct* ShatterStruct;
    char _144[0xc];
    OpalMKProp* OpalMKProps[0xa];
    int OpalCount;
    bool Respawn;
    char _180[0x40];
};


namespace SmashCrate
{
	inline StaticPropDesc GameObj{};
    void InitGameObject(KromeIni* globalModel);
	void HookFunctions();
	void EarlyInit();
};

