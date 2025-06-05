#include "pch.h"
#include "Model.h"

//TygerMemory
#include "core.h"

using TyGetBoundingVolume_t = BoundingVolume*(__thiscall*)(Model* model, int subObjectIndex);
TyGetBoundingVolume_t TyGetBoundingVolume = nullptr;

BoundingVolume* Model::GetBoundingVolume(int subObjectIndex)
{
    if (!TyGetBoundingVolume)
        TyGetBoundingVolume = (TyGetBoundingVolume_t)(Core::moduleBase + 0x1937c0);
    return TyGetBoundingVolume(this, subObjectIndex);
}
