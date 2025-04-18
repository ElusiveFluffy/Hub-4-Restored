#pragma once
#include "GameObject.h"

enum OpalState
{
    Despawned = 0x0,
    Spawning = 0x1,
    Resting = 0x2,
    Magnetized = 0x3,
    SavingPosition = 0x4,
    Collected = 0x5
};

struct OpalMKProp : GameObject::MKProp
{
    char _44[0x34];
    OpalState State;
    char _7c[4];
    char _80[0x40];
    char _c0[0x40];
    char _100[0x14];
};
struct CrateMKProp : GameObject::MKProp
{
    char _44[4];
    bool CollisionEnabled;
    char _49[0x37];
    char _80[0x40];
    char _c0[0x40];
    char _100[0x14];
    BOOL Visible;
    BOOL AllOpalsCollected;
    char _11c[0x24];
    char _140[0x10];
    OpalMKProp* OpalMKProps[0xa];
    int OpalCount;
    char _17c[4];
    char _180[0x40];
};


namespace SmashCrate
{
	inline GameObject::GameObjDesc GameObj{};
	void HookFunctions();
	void EarlyInit();
};

