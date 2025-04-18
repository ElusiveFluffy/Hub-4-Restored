#pragma once
#include "Model.h"
#include "MKMessage.h"

namespace GameObject
{
    struct GameObjDescVTable
    {
        void* Init;
        void* Load;
        void* ConstructObject;
    };
    struct MKPropDescriptor
    {
        char* AliasName;
        float MaxUpdateDist;
        float MaxDrawDist;
        UINT SearchMask;
        int DrawLayer;
        UINT Flags;
        BoundingVolume* pVolume;
        int unk;
        float MaxScissorDist;
        MKPropDescriptor* pNext;
        void* MKProps;
    };
    struct MKPropVTable
    {
        bool (*LoadLine)(void* pLine);
        void (*LoadDone)();
        void (*Reset)();
        void (*Update)();
        void (*Draw)();
        void (*Message)(MKMessage* message);
        void (*Init)();
        void (*Deinit)();
    };
    struct MKProp
    {
        MKPropVTable* VTable;
        MKPropDescriptor* pDescriptor;
        Model* pModel;
        Matrix* pLocalToWorld;
        int Flags;
        int UniqueID;
        char _18[0xc];
        char _24[4];
        char _28[4];
        float DistSquared;
        MKProp* pNext;
        MKProp* pPrev;
        MKProp* pPrevUpdated;
        MKProp* pNextUpdated;
        //Seems to be unused most of the time
        MKProp* pNextOfThisType;
    };

    enum ModuleOverrideFuncFlags
    {
        Module_UpdateOverride = 0x1,
        Module_DrawOverride = 0x2,
        Module_AllocateOverride = 0x8
    };
    struct ModuleInfoVTable
    {
        void(__thiscall* Init)();
        void (*ConstructObject)(void* ptr);
    };
    struct ModuleInfoBaseObject
    {
        void (*InitModule)();
        void (*DeinitModule)();
        void (*pUpdateModule)();
        void (*pDrawModule)();
        int* (*pAllocate)();
        void (*pDeallocate)(void* gameObject);
        int entityCount;
        int InstanceSize;
        bool Update;
        char _21[3];
        ModuleOverrideFuncFlags Flags;
        void* ModuleInfopNext;
    };
    struct ModuleInfo
    {
        ModuleInfoVTable* VTable;
        ModuleInfoBaseObject* pData;
    };

    enum CollisionInfoFlags
    {
        Grabable = 0x1,
        NoIce = 0x2,
        JumpCamera = 0x4
    };
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
    struct StaticPropDescriptor
    {
        char CollideName[0x20];
        bool DynamicCollide;
        bool UseGroundColour;
        char _22[2];
        LODDescriptor lodDesc;
        enum CollisionInfoFlags collisionInfoFlags;
    };
    struct GameObjDesc
    {
        GameObjDescVTable* vTable;
        MKPropDescriptor MkPropDesc;
        char AliasName[0x20];
        char ModelName[0x20];
        ModuleInfo* pModule;
        int InstanceCount;
        MKProp* pInstances;
        char _7c[4];
        char _80[4];
        GameObjDesc* pPrevious;
        //Usually inherits game obj desc 
        //but just doing this to make sure the variable layout is correct
        StaticPropDescriptor StaticPropDesc;
        char _e0[4];
        char _e4[4];
        char _e8[4];
        char _ec[4];
        float field_f0;
    };

    void EarlyInit();
    void APIInit();

    typedef void(__thiscall* GameObjectMsg_t)(MKProp* actor, MKMessage* msg);
    typedef void(__thiscall* GetMKPropRange_t)(GameObjDesc* gameObjDesc, MKProp* pOutProp[2]);
};
