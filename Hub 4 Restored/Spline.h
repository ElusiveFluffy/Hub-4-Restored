#pragma once

//TygerMemory
#include "vector4f.h"

struct UniformSplinePoint
{
    Vector4f Position;
    Vector4f field_10;
    float field_20;
};

struct UniformSpline
{
    int PointCount;
    int NodeIndex;
    float field_8;
    UniformSplinePoint* pPoints;
    bool field_10;
};
