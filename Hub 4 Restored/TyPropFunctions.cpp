#include "pch.h"
#include "TYPropFunctions.h"
#include "core.h"

void TyPropFunctions::AutoTargetSet(int targetPriority, Vector4f* rangTargetPos, Vector4f* biteTargetPos, Vector4f* unkVector, Model* model)
{
	TyAutoTargetSet((void*)(Core::moduleBase + 0x271ee0), targetPriority, rangTargetPos, biteTargetPos, unkVector, model);
}
