#include "pch.h"
#include "Model.h"

//TygerMemory
#include "core.h"

using TyGetBoundingVolume_t = BoundingVolume*(__thiscall*)(Model* model, int subObjectIndex);
using TyGetSubobjectIndex_t = int(__thiscall*)(Model* model, const char* subObjectName);
using TyGetSubobjectMartixIndex_t = int(__thiscall*)(Model* model, int subObjectIndex);
using TyEnableSubObject_t = int(__thiscall*)(Model* model, int subObjectIndex, bool newState);
using TyCreateModel_t = Model*(*)(const char* mdlName, const char* anmName);
using TyDestroyModel_t = void(__thiscall*)(Model* pModel);
using TyDrawModel_t = void(__thiscall*)(Model* pModel, bool arg2);
using TyPostDrawModel_t = void(*)(Model* pModel, float distSquared, int flags);

TyGetBoundingVolume_t TyGetBoundingVolume = nullptr;
TyGetSubobjectIndex_t TyGetSubobjectIndex = nullptr;
TyGetSubobjectMartixIndex_t TyGetSubobjectMartixIndex = nullptr;
TyEnableSubObject_t TyEnableSubObject = nullptr;
TyCreateModel_t TyCreateModel = nullptr;
TyDestroyModel_t TyDestroyModel = nullptr;
TyDrawModel_t TyDrawModel = nullptr;
TyPostDrawModel_t TyPostDrawModel = nullptr;

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

Model* Model::Create(const char* mdlName, const char* anmName)
{
    if (!TyCreateModel)
        TyCreateModel = (TyCreateModel_t)(Core::moduleBase + 0x193200);
    return TyCreateModel(mdlName, anmName);
}

void Model::Destroy()
{
    if (!TyDestroyModel)
        TyDestroyModel = (TyDestroyModel_t)(Core::moduleBase + 0x193540);
    TyDestroyModel(this);
}

void Model::Draw()
{
    if (!TyDrawModel)
        TyDrawModel = (TyDrawModel_t)(Core::moduleBase + 0x1946a0);
    // No idea what the bool is for
    TyDrawModel(this, true);
}

void Model::Draw_AddPostDrawModel(Model* pModel, float distSquared, int flags)
{
    if (!TyPostDrawModel)
        TyPostDrawModel = (TyPostDrawModel_t)(Core::moduleBase + 0x770a0);
    TyPostDrawModel(pModel, distSquared, flags);
}
