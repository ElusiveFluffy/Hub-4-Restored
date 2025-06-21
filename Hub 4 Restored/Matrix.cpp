#include "Matrix.h"

//TygerMemory
#include "core.h"

using TyMatrixScale_t = void(__thiscall*)(Matrix* pThis, Matrix* pMatrix2, Vector4f* scale);
using TyMatrixScale2_t = void(__thiscall*)(Matrix* pThis, Matrix* pMatrix2, float scalar);
using TyMatrixMultiply_t = void(__thiscall*)(Matrix* pThis, Matrix* pMatrix1, Matrix* pMatrix2);
using TyMatrixSetIdentity_t = void(__thiscall*)(Matrix* pThis);

TyMatrixScale_t TyMatrixScale = nullptr;
TyMatrixScale2_t TyMatrixScale2 = nullptr;
TyMatrixMultiply_t TyMatrixMultiply = nullptr;
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

void Matrix::Scale(Matrix* pMatrix, float scalar)
{
	if (!TyMatrixScale2)
		TyMatrixScale2 = (TyMatrixScale2_t)(Core::moduleBase + 0x192370);
	TyMatrixScale2(this, pMatrix, scalar);
}

void Matrix::Multiply(Matrix* pMatrix1, Matrix* pMatrix2)
{
	if (!TyMatrixMultiply)
		TyMatrixMultiply = (TyMatrixMultiply_t)(Core::moduleBase + 0x191e70);
	TyMatrixMultiply(this, pMatrix1, pMatrix2);
}
