#pragma once
#include "ParticleSystem.h"
#include "Model.h"

//TygerMemory
#include "vector4f.h"

struct ShatterStruct
{
    char _0[4];
    char _4[4];
    char _8[4];
    char _c[4];
    ParticleSystem* FireParticleSys;
    Vector4f Position;
    Model* model;
    Vector4f Rotation; //Maybe
    char _38[4];
    char _3c[4];
    char _40[8];
    char _48[4];
    char _4c[4];
    char _50[4];
    bool OnFire;
    bool field_55;
    bool field_56;
    char _57[1];

    void Explode(Vector4f* shatterDir, float minRange, float maxRange);
};

ShatterStruct* Shatter_Add(Model* pModel, float arg2, float arg3, int shatterTickDuration);
