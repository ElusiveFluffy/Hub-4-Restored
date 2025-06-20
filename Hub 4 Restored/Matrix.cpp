#include "Matrix.h"

//TygerMemory
#include "core.h"

using TyMatrixScale_t = void(__thiscall*)(Matrix* pMatrix, Matrix* pMatrix2, Vector4f* scale);
using TyMatrixSetIdentity_t = void(__thiscall*)(Matrix* pMatrix);

TyMatrixScale_t TyMatrixScale = nullptr;
TyMatrixSetIdentity_t TyMatrixSetIdentity = nullptr;

void Matrix::SetIdentity()
{
	if (!TyMatrixSetIdentity)
		TyMatrixSetIdentity = (TyMatrixSetIdentity_t)(Core::moduleBase + 0x192520);
	TyMatrixSetIdentity(this);
}

void Matrix::Scale(Matrix* arg2, Vector4f* scale)
{
	if (!TyMatrixScale)
		TyMatrixScale = (TyMatrixScale_t)(Core::moduleBase + 0x192440);
	TyMatrixScale(this, arg2, scale);
}
