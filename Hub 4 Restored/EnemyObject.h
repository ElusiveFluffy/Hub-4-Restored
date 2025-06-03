#pragma once
#include "AnimatingProp.h"
#include "ActionStateManager.h"

//TygerMemory
#include "vector4f.h"

//Most likely not the right size
TIGHT_PACK_BEGIN
struct EnemyObject : AnimatingProp
{
    ActionStateManager StateManager;
    CollisionObject ColObject;
    char _298[0xc];
    Vector4f Position;
    Vector4f Rotation;
    char _2c4[4];
    char _2c8[0xc];
    char _2d4[4];
    char _2d8[4];
    char _2dc[8];
    char _2e4[4];
    char _2e8[4];
    char _2ec[8];
    char _2f4[4];
    char _2f8[4];
    char _2fc[4];
    Vector4f* SpawnPos;
    char _304[4];
};
TIGHT_PACK_END
