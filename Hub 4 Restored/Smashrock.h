#pragma once
#include "Rangs.h"
#include "vector4f.h"
namespace Smashrock
{
    //Very incomplete, just mainly have the data/offsets I need
    struct ShatterableActorData
    {
        char* pAliasName;
        char _4[0x28];
        char AliasName[0x20];
        char ModelName[0x20];
        char _6c[0x14];
        char _80[4];
        char ColideModelName[0x20];
        char _a4[0x1c];
        char _c0[0x40];
    };
    struct ShatterableActor
    {
        char _0[0x40];
        char _40[0x34];
        Vector4f Position;
    };
    struct ShatterableProp
    {
        void* ShatterableFunctions;
        ShatterableActorData* ActorData;
        ShatterableActor* Actor;
    };

    void UpdateSmashrockValidRang(Rangs::Boomerangs newRang);
    void SmashrockInit();

};

