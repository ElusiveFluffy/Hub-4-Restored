#pragma once
#include "ParticleSystem.h"

struct ParticleSystemGroup
{
    ParticleSystemType SystemType;
    ParticleSystem* pSystem;
    ParticleSystemGroup* pNext;
};
struct ParticleChunkPtrListDL
{
    ParticleChunk** pMem;
};

struct ParticleSystemManager
{
    ParticleSystemGroup* pGroups;
    ParticleChunkPtrListDL ChunkList;
    void* PtrListDLParticleSystem;
    bool DrawDebug;
    int VU_ParticleCount;
    int VU_VirtualParticleCount;
    int VU_ChunkCount;
    int field_1c;
    int CPU_ParticleCount;
    int CPU_VirtualParticleCount;
    int CPU_ChunkCount;
    int field_2c;
    int RejectedSystemCount;
    int field_34;
    UINT MinParticles;
    UINT MaxParticles;
    UINT MinChunks;
    UINT MaxChunks;
    float AverageParticles;
    float AverageChunks;
    void* SystemType;
};
