#include "Material.h"

//TygerMemory
#include "core.h"

using TyMaterialCreate_t = Material*(*)(const char* textureName);
using TyMaterialDestroy_t = void(__thiscall*)(Material* pThis);

TyMaterialCreate_t TyMaterialCreate = nullptr;
TyMaterialDestroy_t TyMaterialDestroy = nullptr;

Material* Material::Create(const char* textureName)
{
    if (!TyMaterialCreate)
        TyMaterialCreate = (TyMaterialCreate_t)(Core::moduleBase + 0x19ee90);
    return TyMaterialCreate(textureName);
}

void Material::Destroy()
{
    if (!TyMaterialDestroy)
        TyMaterialDestroy = (TyMaterialDestroy_t)(Core::moduleBase + 0x19ee90);
    return TyMaterialDestroy(this);
}
