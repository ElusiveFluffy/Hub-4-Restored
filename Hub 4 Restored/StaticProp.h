#pragma once
#include "GameObject.h"
#include "Collision.h"

struct LODEntry
{
    char name[0x20];
    int subObjectFlags;
    int subObjectIndex;
    int unk28;
};
struct LODDescriptor
{
    int Flags;
    LODEntry* pEntries;
    int EntryCount;
    int ShadowFlags;
    int ParticleFlags;
    int SoundFlags;
    int ScissorFlags;
    float Radius;
    float Height;
    int InvisibleZone;
    float MinAlpha;
    float MaxScissorDist;
};
struct LODManager
{
    LODDescriptor* pDescriptor;
    int SubObjectEnableFlags;
    int field_8;
};

#pragma pack(push, 4)
struct StaticProp : GameObject
{
    bool Collide;
    CollisionInfo collisionInfo;
    LODManager Manager;

    virtual bool LoadLine(KromeIniLine* pLine);
    virtual void LoadDone();
    virtual void Draw();
    virtual void Init(GameObjDesc* pDesc);
};
#pragma pack(pop)

inline GameObjDescInit_t TyStaticPropDescInit;
inline GameObjDescLoad_t TyStaticPropDescLoad;
struct StaticPropDesc : GameObjDesc
{
    char CollideName[0x20];
    bool DynamicCollide;
    bool UseGroundColour;
    char _aa[2];
    LODDescriptor lodDesc;
    CollisionInfoFlags collisionInfoFlags;
    void* BadFile;
    char _e4[4];
    float MaxTilt;
    float RotVel;
    float MaxShadowHeight;

    virtual void Init(ModuleInfoBase* moduleInfo, const char* mdlName, const char* aliasName, int searchMask, int flags) {
        TyStaticPropDescInit(this, moduleInfo, mdlName, aliasName, searchMask, flags);
    }
    virtual void Load(KromeIni* pIni) {
        TyStaticPropDescLoad(this, pIni);
    }
};

namespace StaticProps {
    void Init();
}