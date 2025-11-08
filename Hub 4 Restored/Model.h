#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "Animation.h"
#include "Material.h"

//TygerMemory
#include "vector4f.h"
#include "colour.h"

struct BoundingVolume
{
    Vector4f StartPos;
    Vector4f Length;
};

#pragma pack(push, 1)
struct DisplayList {
    BYTE Primitive;
    USHORT VertexCount;
    USHORT VertexIndex;
    USHORT NormalIndex;
    USHORT ColourIndex;
    USHORT UvIndex;
};
#pragma pack(pop)

struct SubObjectMaterial {
    union {
        char* pMaterialName;
        Material* pMaterial;
    };
    DisplayList* pStripData;
    USHORT MaxOffset;
    int StripCount;
};

struct SubObject
{
    BoundingVolume Volume;
    Vector4f Origin;
    char* Name;
    char* Options;
    int FaceCount;
    int Type;
    SHORT MatrixIndex;
    SHORT MaterialCount;
    SubObjectMaterial* SubObjectMaterial;
    float AlphaLightIntensity;
    char _4c[4];
};
struct ModelData
{
    char MDLMagic[0x4];
    SHORT MatrixCount;
    SHORT SubObjectCount;
    SHORT RefPointCount;
    SHORT AnimNodeCount;
    SubObject* SubObjects;
    Vector4f* RefPoints;
    void* AnimNode;
    int field_18;
    int field_1c;
    BoundingVolume BBVolume;
    int DictionaryEntryCount;
    int DictionaryOffset;
    char field_48[0x4];
    int field_4c;
    __time32_t CreationDate;
    int OriginalFileStringOffset;
    int field_58;
    SHORT field_5c;
    SHORT field_5e;
    int field_60;
    int field_64;
    int field_68;
    int field_6c;
};

// Unsure about the name
struct OpenGLModelEntry
{
    Material* pMaterial;
    int32_t Texture1Pos; // Unsure
    int32_t Texture2Pos; // Unsure
    int32_t DrawVertexCount;
};

// Unsure about the name
struct RenderEntry
{
    int EntryCount;
    OpenGLModelEntry* pGLModelEntry;
    char _8[8];
};

struct ModelTemplate
{
    char Name[0x20];
    int ReferenceCount;
    int TemplateDataSize;
    char _28[4];
    ModelData* pModelData;
    RenderEntry* pRenderEntry;
    char _34[0xc];
};

struct Model
{
    ModelTemplate* pTemplate;
    Animation* pAnimation;
    Matrix* pMatrices; // Points to the matrices array in this struct
    char _c[4];
    char _10[4];
    char _14[0x10];
    BYTE* subobjectData;
    int RenderType;
    char _2c[4];
    char _30[4];
    RGBA Colour;
    Matrix Matrices[1]; // Model has at least 1 matrix in it, more are allocated

    BoundingVolume* GetBoundingVolume(int subObjectIndex);
    int GetSubobjectIndex(const char* subObjectName);
    int GetSubObjectMatrixIndex(int subObjectIndex);
    void EnableSubObject(int subObjectIndex, bool newState);
    // Can set anm name to nullptr if it has no anm file
    static Model* Create(const char* mdlName, const char* anmName);
    void Destroy();
    void Draw();
    static void Draw_AddPostDrawModel(Model* pModel, float distSquared, int flags);
};