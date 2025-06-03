#pragma once

//TygerMemory
#include "vector4f.h"

struct Matrix
{
    float Data[0x3][0x4];
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

struct AnimEvent
{
    char* field_0;
    SHORT StartEventFrame;
    SHORT EndEventFrame;
};
struct AnimRange
{
    SHORT StartFrame;
    SHORT EndFrame;
    SHORT Speed;
    SHORT EventCount;
    AnimEvent* pEvents;
    int field_c;
};

struct MKAnim
{
    char* field_0;
    SHORT RangeCount;
    SHORT FrameCount;
    UINT16 field_8;
    UINT8 CycleType;
    UINT8 field_b;
    AnimRange* pAnimRanges;
};
struct MKAnimTemplateSection
{
    char* pMeshName;
    char* pAnimName;
    UINT16 AnimCount;
    UINT16 field_a;
    UINT16 field_c;
    MKAnim* Anims;
};
struct MKAnimScriptTemplate
{
    char Name[0x20];
    int ReferenceCount;
    int TemplateSize;
    int field_28;
    int field_2c;
    MKAnimTemplateSection* pSelection;
};
struct MKAnimScript
{
    MKAnimScriptTemplate* pTemplate;
    MKAnim* CurrAnim;
    MKAnim* NextAnim;
    float field_c;
    float field_10;
    float field_14;
    SHORT field_18;
    SHORT field_1a;
    SHORT TweenBlend; //Not 100% certain
    UINT16 field_1e;
};
