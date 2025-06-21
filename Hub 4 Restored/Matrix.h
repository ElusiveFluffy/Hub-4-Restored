#pragma once

//TygerMemory
#include "vector4f.h"

struct Matrix
{
    float Data[0x3][0x4];
    Vector4f Position;

    void SetIdentity();
    void Scale(Matrix* arg2, Vector4f* scale);
    void Scale(Matrix* pMatrix, float scalar);
    void Multiply(Matrix* pMatrix1, Matrix* pMatrix2);
};
