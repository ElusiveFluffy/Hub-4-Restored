#include "pch.h"
#include "TyRandom.h"

//TygerMemory
#include "core.h"

using TyRandomFR_t = float(*)(int* pSeed, float min, float max);
TyRandomFR_t TyRandomFR = nullptr;

float TyRandom::RandomFR(float min, float max)
{
    return TyRandomFR(RandomSeed, min, max);
}

void TyRandom::InitRandNumFunctions()
{
    RandomSeed = (int*)(Core::moduleBase + 0x288a60);
    TyRandomFR = (TyRandomFR_t)(Core::moduleBase + 0x192cb0);
}
