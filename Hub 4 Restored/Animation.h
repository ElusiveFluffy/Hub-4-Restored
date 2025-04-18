#pragma once

//TygerMemory
#include "vector4f.h"

struct Matrix
{
    float Data[0x4][0x3];
    Vector4f Position;
};
struct AnimationTemplate
{
    char Name[0x20];
    int ReferenceCount;
    void* pAnimData;
};
struct Animation
{
    AnimationTemplate* Template;
    Matrix* Matrices;
};
