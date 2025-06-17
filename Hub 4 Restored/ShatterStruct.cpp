#include "pch.h"
#include "ShatterStruct.h"

//TygerMemory
#include "core.h"

using TyShatterExplode_t = void(__thiscall*)(ShatterStruct* shatterStruct, Vector4f* shatterDir, float minRange, float maxRange);
TyShatterExplode_t TyShatterExplode = nullptr;

using TyShatter_Add_t = ShatterStruct * (*)(Model* pModel, float arg2, float arg3, int shatterTickDuration);
TyShatter_Add_t TyShatter_Add = nullptr;

void ShatterStruct::Explode(Vector4f* shatterUpVelocity, float minRange, float maxRange)
{
	if (!TyShatterExplode)
		TyShatterExplode = (TyShatterExplode_t)(Core::moduleBase + 0x14fc80);
	TyShatterExplode(this, shatterUpVelocity, minRange, maxRange);
}

ShatterStruct* Shatter_Add(Model* pModel, float arg2, float arg3, int shatterTickDuration)
{
	if (!TyShatter_Add)
		TyShatter_Add = (TyShatter_Add_t)(Core::moduleBase + 0x150700);
	return TyShatter_Add(pModel, arg2, arg3, shatterTickDuration);
}
