#pragma once
#include "EnemyObject.h"
#include "Spline.h"
#include "EventMessage.h"
#include "Material.h"
#include "PathManager.h"

//TygerMemory
#include "vector4f.h"

typedef Vector4f* (__thiscall* PipeGetFallingPos_t)(void* pPipe, Vector4f* fallPos);
typedef void(__fastcall* ShadowReset_t)(void* shadow);

namespace ShadowBat
{
    void InitFunctions();

    enum ShadowBatStates {
        ShadowNull,
        StandOnPipe,
        TakeOff,
        HoverIdle,
        Land,
        ReturnToHome,
        KissOfDeathChase,
        KissOfDeath,
        KissOfDeathEnd,
        WhiteBatAttack,
        SonarAttack,
        Steam,
        Fall,
        FlyToNextPipe
    };
};

enum ShadowBatActionState
{
    NoAction = 0x0,
    Attack = 0x1,
    Movement = 0x2
};

TIGHT_PACK_BEGIN
struct ShadowBatProp : EnemyObject
{
    char _308[0x38];
    char _340[0x40];
    char _380[0x10];
    char _390[4];
    BOOL NoStateManager;
    char _398[4];
    char _39c[0x24];
    char _3c0[0x40];
    char _400[0x1c];
    int Health;
    float SplineLerpAlpha;
    float SplineMovementSpeed;
    char _428[4];
    float YawAngle;
    float PitchAngle;
    char _434[4];
    char _438[8];
    int BatBounceCount;
    float RandomFloat;
    char _448[4];
    Vector4f FallStartPos;
    Vector4f FallPosition;
    UniformSpline SplinePoints;
    char _47d[3];
    char _480[4];
    char _484[4];
    char _488[4];
    char _48c[4];
    char _490[8];
    char _498[4];
    char _49c[4];
    char _4a0[4];
    char _4a4[8];
    char _4ac[4];
    char _4b0[4];
    char _4b4[4];
    char _4b8[8];
    char _4c0[0x10];
    char _4d0[4];
    char _4d4[4];
    char _4d8[0xc];
    char _4e4[4];
    char _4e8[4];
    char _4ec[4];
    char _4f0[4];
    char _4f4[4];
    char _4f8[8];
    char _500[4];
    ShadowBatActionState ActionState;
    StaticProp* pShadowBatPipe1;
    StaticProp* pShadowBatPipe2;
    StaticProp* pShadowBatPipe3;
    char _514[0x2c];
    char _540[0x18];
    char _558[0x28];
    char _580[0x40];
    char _5c0[0x40];
    char _600[0x40];
    char _640[0x24];
    char _664[0x14];
    char _678[4];
    char _67c[4];
    char _680[0x40];
    char _6c0[0x40];
    char _700[0x40];
    char _740[0x40];
    char _780[0x40];
    char _7c0[0x40];
    char _800[0x40];
    char _840[0x40];
    char _880[0x40];
    char _8c0[0x40];
    char _900[0x40];
    char _940[0x40];
    char _980[0x40];
    char _9c0[0x40];
    char _a00[0x40];
    char _a40[0x40];
    char _a80[0x40];
    char _ac0[0x1c];
    StaticProp* Stalactites;
    char _ae0[0x20];
    char _b00[0x20];
    char _b20[0x20];
    char _b40[0x40];
    char _b80[0x40];
    char _bc0[0x40];
    char _c00[0x40];
    char _c40[0x40];
    char _c80[0x40];
    char _cc0[0x40];
    char _d00[0x40];
    char _d40[0x40];
    char _d80[0x40];
    char _dc0[0x40];
    char _e00[0x40];
    char _e40[0x40];
    char _e80[0x40];
    char _ec0[0x40];
    char _f00[0x40];
    char _f40[0x40];
    char _f80[0x40];
    char _fc0[0x40];
    char _1000[0x40];
    char _1040[0x40];
    char _1080[0x24];
    PathManager PathManager;
    EventMessage OnDefeated;
    void* SoundBank;
    Material* ShadowCameoMat;
};
TIGHT_PACK_END
