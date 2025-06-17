#include "pch.h"
#include "Model.h"

//TygerMemory
#include "core.h"

using TyGetBoundingVolume_t = BoundingVolume*(__thiscall*)(Model* model, int subObjectIndex);
using TyGetSubobjectIndex_t = int(__thiscall*)(Model* model, const char* subObjectName);
using TyGetSubobjectMartixIndex_t = int(__thiscall*)(Model* model, int subObjectIndex);
using TyEnableSubObject_t = int(__thiscall*)(Model* model, int subObjectIndex, bool newState);

TyGetBoundingVolume_t TyGetBoundingVolume = nullptr;
TyGetSubobjectIndex_t TyGetSubobjectIndex = nullptr;
TyGetSubobjectMartixIndex_t TyGetSubobjectMartixIndex = nullptr;
TyEnableSubObject_t TyEnableSubObject = nullptr;

BoundingVolume* Model::GetBoundingVolume(int subObjectIndex)
{
    if (!TyGetBoundingVolume)
        TyGetBoundingVolume = (TyGetBoundingVolume_t)(Core::moduleBase + 0x1937c0);
    return TyGetBoundingVolume(this, subObjectIndex);
}

int Model::GetSubobjectIndex(const char* subObjectName)
{
    if (!TyGetSubobjectIndex)
        TyGetSubobjectIndex = (TyGetSubobjectIndex_t)(Core::moduleBase + 0x193980);
    return TyGetSubobjectIndex(this, subObjectName);
}

int Model::GetSubObjectMatrixIndex(int subObjectIndex)
{
    if (!TyGetSubobjectMartixIndex)
        TyGetSubobjectMartixIndex = (TyGetSubobjectMartixIndex_t)(Core::moduleBase + 0x193a20);
    return TyGetSubobjectMartixIndex(this, subObjectIndex);
}

void Model::EnableSubObject(int subObjectIndex, bool newState)
{
    if (!TyEnableSubObject)
        TyEnableSubObject = (TyEnableSubObject_t)(Core::moduleBase + 0x1935c0);
    TyEnableSubObject(this, subObjectIndex, newState);
}
