#pragma once
#include "Model.h"
#include "GameObject.h"

//TygerMemory
#include "vector4f.h"
#include "colour.h"

enum CollisionInfoFlags
{
    Grabable = 0x1,
    NoIce = 0x2,
    JumpCamera = 0x4
};

enum CollisionMode
{
    COLLISION_MODE_ALL = 0x0,
    COLLISION_MODE_DYNAMIC = 0x1,
    COLLISION_MODE_POLY = 0x2
};

#pragma pack(push, 4)
struct TyCollisionInfo
{
    bool Enabled;
};

struct CollisionInfo
{
    bool Enabled;
    CollisionInfoFlags Flags;
    MKProp* pProp;
};

struct CollisionResult
{
    Vector4f Pos;
    Vector4f Normal;
    RGBA Colour;
    Model* pModel;
    int ItemIdx;
    UINT CollisionFlags;
    CollisionInfo* pInfo;
    float field_40;
};

struct ColObjDescriptorSubStruct
{
    float field_0;
    float field_4;
    int field_8;
};
struct ColObjDescriptor
{
    float Radius;
    float yOffset;
    CollisionMode Mode;
    int VelocityCalSettings;
    int ResolveIterationCount;
    char field_14;
    int IgnoreMatID;
    ColObjDescriptorSubStruct field_1c[0x3];
    int field_40;
};

struct ContextInfo
{
    int ValidCount;
    CollisionResult Results[0x2];
    int VelocitySettings;
};

struct CollisionObject
{
    ContextInfo Contexts[0x3];
    Vector4f Pos;
    ColObjDescriptor* pColObjDesc;
    char ColObjCount;
    CollisionObject** ppColObjects;
    TyCollisionInfo* pTyColInfo;
    Vector4f field_1d0;
    Vector4f Pos1;
};
#pragma pack(pop)