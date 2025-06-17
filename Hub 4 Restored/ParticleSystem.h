#pragma once
#include "Model.h"
#include "vector3f.h"

struct ParticleSystemManager;
struct Particle
{
    Vector3f Coord;
    float Age;
    RGBA Colour;
    Vector3f Dir;
    int EnvelopeIndex;
    float field_30;
    float field_34;
    float Angle;
    float field_3c;
    float field_40;
    float field_44;
    float field_48;
    float field_4c;
    float field_50;
    float field_54;
    union
    {
        float floatData[0x2];
        char charData[0x2];
    };
};


struct ParticleChunk
{
    Particle ChunkData[0x18];
    int DataIndex;
    ParticleChunk* pNext;
};

struct DynamicData
{
    float unk0;
    float unk4;
    Matrix* pMatrix;
    float unkC;
    bool unk10;
};

struct ParticleSystemType
{
    char _0[1];
    char _1[3];
    char* pName;
    char _8[8];
    char _10[4];
    char _14[0xc];
    char _20[4];
    char _24[4];
    char _28[4];
    char _2c[8];
    char _34[4];
    char _38[4];
    char _3c[4];
    char _40[0x14];
    char _54[0x10];
    char _64[8];
    char _6c[8];
};

struct ParticleSystem
{
    ParticleSystemType* SystemType;
    DynamicData* pDynamicData;
    int DynamicDataCount;
    Vector4f* pPos;
    BoundingVolume Volume;
    float field_30;
    float Scale;
    float field_38;
    float field_3c;
    int field_40;
    float UOffset;
    float VOffset;
    float TextureIndexOffset;
    ParticleSystemManager* pSystemManager;
    DynamicData dynamicData;
    ParticleChunk* pChunks;
    int ParticleChunkCount;
    int LiveParticleCount;
    bool field_74;
    bool field_75;
    float field_78;
    bool IsDying;
    float DeathTimer;
    float InvDeathTimer;
    float Age;
    ParticleSystem* pNext;
    int UniqueID;

    void Destroy();
    void Destroy(float deathTimer);
};
