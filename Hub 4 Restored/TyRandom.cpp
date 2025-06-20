#include "pch.h"
#include "TyRandom.h"

//TygerMemory
#include "core.h"

using TyRandomFR_t = float(*)(int* pSeed, float min, float max);
using TyRandomIR_t = int(*)(int* pSeed, int min, int max);

TyRandomFR_t TyRandomFR = nullptr;
TyRandomIR_t TyRandomIR = nullptr;

float TyRandom::RandomFR(float min, float max)
{
    return TyRandomFR(RandomSeed, min, max);
}

int TyRandom::RandomIR(int inclusiveMin, int exclusiveMax)
{
    return TyRandomIR(RandomSeed, inclusiveMin, exclusiveMax);
}

void TyRandom::InitRandNumFunctions()
{
    RandomSeed = (int*)(Core::moduleBase + 0x288a60);
    TyRandomFR = (TyRandomFR_t)(Core::moduleBase + 0x192cb0);
    TyRandomIR = (TyRandomIR_t)(Core::moduleBase + 0x192d20);
}
