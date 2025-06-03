#pragma once
#include "vector4f.h"
#include "Model.h"

namespace TyPropFunctions
{
	using TyAutoTargetSet_t = void(__thiscall*)(void* autoTargetStruct, int targetPriority, Vector4f* rangTargetPos, Vector4f* biteTargetPos, Vector4f* unkVector, Model* model);
	inline TyAutoTargetSet_t TyAutoTargetSet;

	void AutoTargetSet(int targetPriority, Vector4f* rangTargetPos, Vector4f* biteTargetPos, Vector4f* unkVector, Model* model);
};

