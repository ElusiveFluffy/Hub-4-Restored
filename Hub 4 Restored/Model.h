#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "Animation.h"

//TygerMemory
#include "vector4f.h"
#include "colour.h"

struct BoundingVolume
{
    Vector4f v1;
    Vector4f v2;
};
struct ModelData
{
    char ID[0x4];
    SHORT MatrixCount;
    SHORT SubObjectCount;
    SHORT RefPointCount;
    SHORT AnimNodeCount;
    struct SubObject* SubObjects;
    struct Vector4* Unk;
    void* Vertex;
    int VertexCount;
    BoundingVolume Volume;
};
struct ModelTemplate
{
    char Name[0x20];
    int ReferenceCount;
    int TemplateDataSize;
    char _28[4];
    ModelData* pModelData;
    char _30[0x10];
};

struct Model
{
    ModelTemplate* pTemplate;
    Animation* pAnimation;
    Matrix* pMatrices;
    char _c[0x28];
    RGBA Colour;
    Matrix Matrices;
};